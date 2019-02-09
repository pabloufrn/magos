#include <cstdlib> // EXIT_SUCCESS
#include <iostream> // cerr
#include <fstream> // ifstream

#include "../include/magosgame.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main( int argc, char *argv[] )
{
    // Load options
    size_t rows, cols, width, height, margin;
    po::variables_map vm;
    try {
        std::string config_file;
        // Declare a group of options that will be 
        // allowed only on command line
        po::options_description cmd("General options");
        cmd.add_options()
            ("help,h", "produce help message")
            ("config,c", po::value<std::string>(&config_file)->default_value("autoconfig.cfg"),
                  "name of a file of a configuration")
            ("margin,m", po::value<size_t>(&margin)->default_value(50),
                "minimum distance from the maze to the border")
            ;
    
        // Declare a group of options that will be 
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()
            ("output_builder_path", po::value<std::string>()->default_value("builder"),
             "builder output images directory")
            ("output_solver_path", po::value<std::string>()->default_value("solver"),
             "solver output images directory")
            ("output_building_file_format", po::value<std::string>()->default_value("building_$"),
             "format of building images ($=step number)")
            ("output_solving_file_format", po::value<std::string>()->default_value("solving_$"),
             "format of solving images ($=step number)")
            ;

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("maze-rows", po::value<size_t>(&rows)->default_value(20))
            ("maze-cols", po::value<size_t>(&cols)->default_value(30))
            ("image-width", po::value<size_t>(&width)->default_value(1280))
            ("image-height", po::value<size_t>(&height)->default_value(720))
            ("background-color", po::value<std::string>()) 
            ("wall-color", po::value<std::string>()) 
            ("entrance_point-color", po::value<std::string>()) 
            ("exit_point-color", po::value<std::string>()) 
            ("path_point-color", po::value<std::string>()) 
            ("discarted_point-color", po::value<std::string>()) 
            ;

        
        po::options_description cmdline_options;
        cmdline_options.add(cmd).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(cmd).add(config);
        
        po::positional_options_description p;
        p.add("maze-rows", 1);
        p.add("maze-cols", 1);
        p.add("image-width", 1);
        p.add("image-height", 1);
        
        store(po::command_line_parser(argc, argv).
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            std::cout << "Usage: magos [maze rows] [maze cols] [image width] [image height] [OPTION]...\n\n";
            std::cout << visible << "\n";
            std::cout << "Example: magos 20 10 800 600 -c config.cfg\n";
            return 0;
        }
        
        std::ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            std::cout << "[Warning] can not open config file: " << config_file 
                << ". Default values will be used.\n";
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
    
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    mzr::MagosGame mg;

    mg.load_colors(vm, [](po::variable_value& vv){return vv.as<std::string>();});
    mg.welcome();

    auto result = mg.initialize( rows, cols, width, height, margin);
    if ( result.type == mzr::MagosGame::game_state_e::ERROR )
    {
        std::cerr << result.msg << "\n";
        return EXIT_FAILURE;
    }

    mg.set_builder_output_path(vm["output_builder_path"].as<std::string>());
    mg.set_solver_output_path(vm["output_solver_path"].as<std::string>());
    mg.set_building_output_format(vm["output_building_file_format"].as<std::string>());
    mg.set_solving_output_format(vm["output_solving_file_format"].as<std::string>());

    // The Game Loop.
    
    while( not mg.game_over() )
    {
        mg.process_events();
        mg.update();
        mg.render();
    }

    return EXIT_SUCCESS;
}
