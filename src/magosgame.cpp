#include "../include/magosgame.h"
#include <sstream>
#include <iomanip>

using namespace mzr;

MagosGame::~MagosGame()
{
    delete m_mz;
    delete m_render;
    delete m_solver;
    delete m_builder;
}

void MagosGame::welcome()
{
    std::cout 
        << "*--------------------*" << std::endl
        << "|   Bem Vindo ao     |" << std::endl
        << "|    Magos Game      |" << std::endl 
        << "*--------------------*" << std::endl;
    m_wait_user = true;
}
MagosGame::Result MagosGame::initialize(size_t rows, size_t cols, size_t width, size_t height, size_t margin)
{
    m_mz = new Maze(cols, rows);
    m_render = new Render(m_mz, width, height, margin);
    m_builder = new Builder(m_mz);
    m_solver = new Solver(m_mz);
    m_state = MagosGame::game_state::BUILDING;
    m_builder_path = "builder";
    m_solver_path = "solver";
    m_solving_prefix = "solving_";
    m_solving_suffix = ".png";
    m_building_prefix = "building_";
    m_building_suffix = ".png";
    m_state_trasition = true;
    if(m_colors == nullptr)
        m_colors->load_default();
    m_render->initialize_colors(m_colors);
    return Result{MagosGame::game_state_e::OK, "OK"};
}

void MagosGame::update()
{
    if(m_state == MagosGame::game_state::BUILDING)
    {
        if(m_state_trasition)
        {
            m_builder->initalize();
            std::cout << "[!] Building process has been started." << std::endl;
            m_state_trasition = false;
        }
        else{
            m_builder->next_step();
            if(m_builder->is_finished())
            {
                std::cout << "[!] Building process has been finished." << std::endl;
                m_state = MagosGame::game_state::SOLVING;
                m_state_trasition = true;
                m_wait_user = true;
            }
        }
    }
    else if(m_state == MagosGame::game_state::SOLVING)
    {
        if(m_state_trasition)
        {
            m_solver->initialize();
            std::cout << "[!] Solving process has been started." << std::endl;
            m_state_trasition = false;
        }
        else
        {
            m_solver->next_step();
            if(m_solver->is_finished())
            {
                std::cout << "[!] Solving process has been finished." << std::endl;
                m_state = MagosGame::game_state::FINISHED;
                m_state_trasition = true;
            }
        }
    }
}

void MagosGame::render()
{
    m_render->update_canvas();
    if(m_state == MagosGame::game_state::BUILDING)
    {
        std::ostringstream oss;
        std::ostringstream step_number;
        step_number << std::setw(5) << std::setfill('0') << std::to_string(m_builder->get_step_number());
        oss << m_builder_path << "/" << m_building_prefix 
            << step_number.str()
            << m_building_suffix << ".png";
        m_render->draw(oss.str());
    }
    else if(m_state == MagosGame::game_state::SOLVING)
    {
        if(m_state_trasition)
            return;
        std::ostringstream oss;
        std::ostringstream step_number;
        step_number << std::setw(5) << std::setfill('0') << std::to_string(m_solver->get_step_number());
        oss << m_solver_path << "/" << m_solving_prefix 
            << step_number.str()
            << m_solving_suffix << ".png";
        m_render->draw(oss.str());
    }
    else if(m_state_trasition and m_state == MagosGame::game_state::FINISHED)
    {
        std::ostringstream oss;
        std::ostringstream step_number;
        step_number << std::setw(5) << std::setfill('0') << std::to_string(m_solver->get_step_number());
        oss << m_solver_path << "/" << m_solving_prefix 
            << step_number.str()
            << m_solving_suffix << ".png";
        m_render->draw(oss.str());
    }
}

bool MagosGame::game_over()
{
    return m_state == MagosGame::game_state::FINISHED;
}

void MagosGame::process_events()
{
    if(m_wait_user)
    {
        std::cout << "*-> Aperte enter para continuar...";
        std::cin.ignore();
        m_wait_user = false;
    }
}
void MagosGame::set_building_output_format(std::string format)
{
    // validate format 
    int sign_pos = -1;
    for(auto i = 0u; i < format.length(); ++i){
        auto & c = format[i];
        if(c == '/' or c == '\\')
            throw std::runtime_error("invalid format");
        if(c == '$')
        {   
            if(sign_pos != -1)
                throw std::runtime_error("invalid format");
            else
                sign_pos = i; 
        }
    }
    m_building_prefix = format.substr(0, sign_pos);
    m_building_suffix = format.substr(sign_pos+1);
}
void MagosGame::set_solving_output_format(std::string format){
    // validate format 
    int sign_pos = -1;
    for(auto i = 0u; i < format.length(); ++i){
        auto & c = format[i];
        if(c == '/' or c == '\\' or c == '\n')
            throw std::runtime_error("invalid format");
        if(c == '$')
        {   
            if(sign_pos != -1)
                throw std::runtime_error("invalid format");
            else
                sign_pos = i; 
        }
    }
    m_solving_prefix = format.substr(0, sign_pos);
    m_solving_suffix = format.substr(sign_pos+1);
}
void MagosGame::set_solver_output_path(std::string path){
   m_solver_path = path;
}
void MagosGame::set_builder_output_path(std::string path){
    m_builder_path = path;
}
