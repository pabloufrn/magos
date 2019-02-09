#ifndef COLOR_CONFIG_H
#define COLOR_CONFIG_H

#include "common.h"
#include <map>

namespace mzr
{
    class ColorConfig
    {
        public:
            ColorConfig() : m_loaded(false), m_config(std::map<std::string, canvas::Color>()) {}

            template < typename T , typename ConvertStringFunction>
            void load_from_map(std::map<std::string, T>& map, const ConvertStringFunction& csf)
            {
                std::string proprieties[6] = {
                    "background-color", "wall-color", "entrance_point-color", 
                    "exit_point-color", "path_point-color", "discarted_point-color"};
                canvas::Color default_values[6]  =  {
                    canvas::LIGHT_GREY, canvas::BLACK, canvas::LIGHT_BLUE,
                    canvas::GREEN, canvas::CRIMSON, canvas::YELLOW
                };
                std::string current_prop;
                std::string current_value;
                for(auto i = 0u; i < 6; i++)
                {
                    current_prop = proprieties[i];
                    try
                    {
                        current_value = csf(map.at(current_prop)); 
                        m_config[current_prop] = canvas::Color::from_hex_str(current_value);
                    } catch(std::out_of_range& ex)
                    {
                        std::cout << "[WARNIG] color '" << current_prop << "' not found. Using default value.\n";
                        m_config[current_prop] = default_values[i];
                    }
                    catch(std::exception& ex)
                    {
                        throw std::runtime_error("Invalid '" + current_prop + "'");
                    }
                }
                m_loaded = true;
            }

            bool is_loaded() const
            {
                return m_loaded;
            }

            void load_default()
            {
                m_config = {
                   {"background-color", canvas::LIGHT_GREY}, {"wall-color", canvas::BLACK}, {"entrance_point-color", canvas::LIGHT_BLUE},
                   {"exit_point-color", canvas::GREEN}, {"path_point-color", canvas::CRIMSON}, {"discarted_point-color", canvas::YELLOW}};
                m_loaded = true;
            }

            canvas::Color& at(std::string s)
            {
                if(!m_loaded)
                    throw std::runtime_error("unloaded config.");
                return m_config.at(s);
            }
            
       private:
            bool m_loaded;
            std::map<std::string, canvas::Color> m_config;
    };
}

#endif // COLOR_CONFIG_H
