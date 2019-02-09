#include "../include/builder.h"

using namespace mzr;

void Builder::initalize()
{
    if(m_finished)
        throw std::runtime_error("the building process has already started.");
    m_cells_length = m_mz->get_height()* m_mz->get_width();
    m_rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_cells_length);
    m_current_cell_index = dist(m_rng);

    m_mz->get_cell(m_current_cell_index).set_bit(Maze::BUILT);
    m_finished = false;
    m_step_counter = 0;
}

bool Builder::is_finished()
{
    return m_finished;
}

size_t Builder::get_step_number()
{
    return m_step_counter;
}

void Builder::next_step()
{
    if(m_finished)
        throw std::runtime_error("the building process has already finished.");
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 3);
    auto side = dist(m_rng);
    auto width = m_mz->get_width();
    bool found_side = false;
    size_t next_cell_index;
    Cell next_cell;
    for(auto i = 0u; i < 4; ++i, ++side)
    {
        if(side == 4)
            side = 0;
        if(side == Maze::side::TopWall and m_current_cell_index >= width)
        {
            next_cell_index = m_current_cell_index - width;
            next_cell = m_mz->get_cell(next_cell_index);
            if(!next_cell.get_bit(Maze::BUILT))
                found_side = true;
        }
        if(side == Maze::side::RightWall and (m_current_cell_index  % width) != width - 1)
        {
            next_cell_index = m_current_cell_index + 1;
            next_cell = m_mz->get_cell(next_cell_index);
            if(!next_cell.get_bit(Maze::BUILT))
                found_side = true;
        }
        if(side == Maze::side::BottomWall and m_current_cell_index + width < m_cells_length)
        {
            next_cell_index = m_current_cell_index + width;
            next_cell = m_mz->get_cell(next_cell_index);
            if(!next_cell.get_bit(Maze::BUILT))
                found_side = true;
        }
        if(side == Maze::side::LeftWall and (m_current_cell_index  % width) != 0)
        {
            next_cell_index = m_current_cell_index - 1;
            next_cell = m_mz->get_cell(next_cell_index);
            if(!next_cell.get_bit(Maze::BUILT))
                found_side = true;
        }
        if(found_side)
        {
            m_stack.push(m_current_cell_index);
            m_mz->knock_down(m_current_cell_index % width, m_current_cell_index / width, (Maze::side) side);
            m_current_cell_index = next_cell_index;
            m_mz->get_cell(m_current_cell_index).set_bit(Maze::BUILT);
            m_step_counter++;
            return;
        }
    }
    if(!m_stack.empty())
    {
        m_current_cell_index = m_stack.top();
        m_stack.pop();
        return this->next_step();
    }
    m_finished = true;
}

