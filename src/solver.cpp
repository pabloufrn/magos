#include "../include/solver.h"

using namespace mzr;

void Solver::initialize()
{
    m_current_cell_index = 0;
    m_target_index = m_mz->get_width()*m_mz->get_height() - 1;
    m_step_counter = 0;
    m_finished = false;
    Cell& current_cell = m_mz->get_cell(m_current_cell_index);
    current_cell.set_bit(Maze::VISITED);
    current_cell.set_bit(Maze::PATH);
    m_stack.push(m_current_cell_index);
}

void Solver::next_step()
{
    if(m_finished)
        throw std::runtime_error("Try to next_step() in a finished solver.");
    if(m_current_cell_index == m_target_index)
    {
        m_finished = true;
        return;
    }


    m_step_counter++;

    auto width = m_mz->get_width();

    // try next unvisited neighbor
    for(auto side = 0u; side < 4; ++side)
    {
        if(this->is_valid_side((Maze::side) side))
        {
            if(side == Maze::side::TopWall and 
                    not m_mz->get_cell(m_current_cell_index - width)
                    .get_bit(Maze::VISITED))
            {
                m_stack.push(m_current_cell_index);
                m_current_cell_index -= width;
                Cell& current_cell = m_mz->get_cell(m_current_cell_index);
                current_cell.set_bit(Maze::VISITED);
                current_cell.set_bit(Maze::PATH);
                return;
            }
            else if(side == Maze::side::RightWall and
                    not m_mz->get_cell(m_current_cell_index+1)
                    .get_bit(Maze::VISITED))
            {
                m_stack.push(m_current_cell_index);
                m_current_cell_index++;
                Cell& current_cell = m_mz->get_cell(m_current_cell_index);
                current_cell.set_bit(Maze::VISITED);
                current_cell.set_bit(Maze::PATH);
                return;
            }
            else if(side == Maze::side::BottomWall and
                    not m_mz->get_cell(m_current_cell_index+width)
                    .get_bit(Maze::VISITED))
            {
                m_stack.push(m_current_cell_index);
                m_current_cell_index += width;
                Cell& current_cell = m_mz->get_cell(m_current_cell_index);
                current_cell.set_bit(Maze::VISITED);
                current_cell.set_bit(Maze::PATH);
                return;
            }
            else if(side == Maze::side::LeftWall and
                    not m_mz->get_cell(m_current_cell_index-1)
                    .get_bit(Maze::VISITED))
            {
                m_stack.push(m_current_cell_index);
                m_current_cell_index -= 1;
                Cell& current_cell = m_mz->get_cell(m_current_cell_index);
                current_cell.set_bit(Maze::VISITED);
                current_cell.set_bit(Maze::PATH);
                return;
            }
        }
    }
    if(!m_stack.empty())
    {
        m_mz->get_cell(m_current_cell_index).unset_bit(Maze::PATH);
        m_current_cell_index = m_stack.top();
        m_stack.pop();
    }
    else
    {
        throw std::runtime_error("Path no found");
    }
}
bool Solver::is_valid_side(Maze::side s)
{
    return not m_mz->get_cell(m_current_cell_index).get_bit(s);
}
bool Solver::is_finished()
{
    return m_finished;
}
size_t Solver::get_step_number()
{
    return m_step_counter;
}
std::stack<size_t> Solver::get_path()
{
    if(!m_finished)
        std::runtime_error("getting path of unfinished solution");
    return m_stack;
}
