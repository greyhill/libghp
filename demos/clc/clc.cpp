#include <ghp/util/cl.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
  namespace po = boost::program_options;
  namespace fs = boost::filesystem;

  po::options_description desc("Allowed options");
  desc.add_options()
      ("input,i", po::value<std::string>(), "input file")
      ("output,o", po::value<std::string>(), "output file")
      ("cpp,x", "produce cpp/header file on successful compilation")
      ("platform,p", po::value<unsigned>(), "set platform")
      ("device,d", po::value<unsigned>(), "set device")
      ("options,c", po::value<std::string>(), "compile options")
      ("help,h", "print this help message")
      ("list,l", "list platforms and devices");

  po::positional_options_description pos;
  pos.add("input", 1);
  pos.add("output", 1);

  po::variables_map vm;
  try {
    po::store(
        po::command_line_parser(argc, argv).options(desc)
            .positional(pos).run(), vm);
  } catch(std::exception &e) {
    std::cerr << "options parsing error.\n" << desc << std::endl;
    return EXIT_FAILURE;
  }
  po::notify(vm);

  if(vm.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }

  if(vm.count("list")) {
    std::cout << "Accessible platforms:\n";
    std::vector<cl::platform_ref> platforms;
    cl::platform_ref::get_platforms(platforms);
    for(unsigned i=0; i<platforms.size(); ++i) {
      cl::platform_ref p = platforms[i];
      std::cout << i << ") " << p.name() << "\n";

      std::cout << "\tAssociated devices:\n";
      std::vector<cl::device_ref> devices;
      p.get_devices(devices);
      for(unsigned j=0; j<devices.size(); ++j) {
        cl::device_ref d = devices[j];
        std::cout << "\t" << j << ") " << d.name() << "\n";
      }
    }
    return EXIT_SUCCESS;
  }

  std::string input_path;
  std::string output_path;
  std::string options;
  bool do_output = false;
  unsigned platform_id = 0;
  unsigned device_id = 0;

  if(vm.count("input")) {
    input_path = vm["input"].as<std::string>();
  } else {
    std::cout << "must provide input file.\n";
    return EXIT_FAILURE;
  }
  if(vm.count("output")) {
    output_path = vm["output"].as<std::string>();
    do_output = true;
  }
  if(vm.count("platform")) {
    platform_id = vm["platform"].as<unsigned>();
  }
  if(vm.count("device")) {
    device_id = vm["device"].as<unsigned>();
  }
  if(vm.count("options")) {
    options = vm["options"].as<std::string>();
  }

  std::stringstream ss;
  std::ifstream in(input_path.c_str());
  std::string line;
  while(std::getline(in, line)) {
    ss << line << "\n";
  }

  std::vector<cl::platform_ref> platforms;
  std::vector<cl::device_ref> devices;
  cl::platform_ref::get_platforms(platforms);

  if(platform_id >= platforms.size()) {
    std::cerr << "invalid platform id\n";
    return EXIT_FAILURE;
  }
  cl::platform_ref platform = platforms[platform_id];
  platform.get_devices(devices);

  if(device_id >= devices.size()) {
    std::cerr << "invalid device id\n";
    return EXIT_FAILURE;
  }
  cl::device_ref device = devices[device_id];

  cl::context_ref context(platform, device);
  cl::program_ref program(context, ss.str());
  std::cout << "context is " << platform.name() << "/"
    << device.name() << "\n";
  std::cout << "OpenCL compiler options: " << options << "\n";
  try {
    program.build(options);
    std::cout << "build successful!\n";

    if(vm.count("header")) {
      fs::path input_path_obj(input_path);
      std::string file_base = input_path_obj.stem();
      std::string file_base_upper = file_base;
      boost::to_upper(file_base_upper);
  
      // write header
      std::stringstream filess;
      filess << input_path << ".hpp";
      std::ofstream header_out(filess.str().c_str());
      filess.str("");
      filess << "#ifndef _" << file_base_upper << "_OPENCL_HPP_" << "\n";
      filess << "#define _" << file_base_upper << "_OPENCL_HPP_" << "\n";
      filess << "\n";
      filess << "extern const char *" << file_base << "_opencl_source;" << "\n";
      filess << "\n";
      filess << "#endif\n\n";
      header_out << filess.str();
      header_out.close();
  
      // write cpp file
      boost::regex rx;
      rx.assign(
        "(\\\\)|"
        "(\")");
      const char *format = 
        "(?1\\\\\\\\)"
        "(?2\\\\\")";

      filess.str("");
      filess << input_path << ".cpp";
      std::ofstream source_out(filess.str().c_str());
      std::ifstream source_in(input_path.c_str());
      filess.str("");
      filess << "const char *" << file_base << "_opencl_source = " << "\n";
      while(getline(source_in, line)) {
        std::string sanitized_line = 
          boost::regex_replace(line, rx, format, 
            boost::match_default | boost::format_all);
        filess << "\t\"" << sanitized_line << "\\n\"\n";
      }
      filess << ";";
      source_out << filess.str();
      source_out.close();
    }
  } catch(const cl::cl_error &c) {
    std::cout << "error building program\n";
  }

  std::cout << "build log:\n" << program.get_build_log(device)
    << "\n";

  return EXIT_SUCCESS;
}

