#include "../include/maze.h"

using namespace mzr;

void Cell::set_bit(unsigned char pos)
{
    m_data |= (1u << pos);
}
void Cell::unset_bit(unsigned char pos)
{
    m_data &= ~(1u << pos);
}
bool Cell::get_bit(unsigned char pos)
{
    return (m_data >> pos) & 1u;
}
void Cell::set_data(cell_t data)
{
    m_data = data;
}
Cell::cell_t Cell::get_data()
{
    return m_data;
}
void Maze::knock_down(size_t x, size_t y , side s)
{
    size_t cell_index = y*m_width + x;
    m_cells[cell_index].unset_bit((char) s);
    if(s == side::LeftWall and x > 0)
        m_cells[cell_index-1].unset_bit(side::RightWall);
    else if(s == side::TopWall and y > 0)
        m_cells[cell_index-m_width].unset_bit(side::BottomWall);
    else if(s == side::RightWall and x < m_width - 1)
        m_cells[cell_index+1].unset_bit(side::LeftWall);
    else if(s == side::BottomWall and y < m_height-1)
        m_cells[cell_index+m_width].unset_bit(side::TopWall);
}
Cell & Maze::get_cell(size_t x, size_t y) const
{
    return m_cells[y * m_width + x];
}
Cell & Maze::get_cell(size_t index) const
{
    return m_cells[index];
}

