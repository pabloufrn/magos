#ifndef M_SOLVER_H
#define M_SOLVER_H

#include <cstddef>
#include <stack>
#include <stdexcept>
#include <iostream>
#include "maze.h"

namespace mzr
{
    class Solver
    {
        public:
            Solver(Maze * mz) : m_mz(mz) {}
            void initialize();
            void next_step();
            bool is_valid_side(Maze::side s);
            bool is_finished();
            size_t get_step_number();
            std::stack<size_t> get_path();

        private:
            Maze* m_mz;
            size_t m_step_counter;
            size_t m_current_cell_index;
            bool m_finished;
            size_t m_target_index;
            std::stack<size_t> m_stack;
    };
}
#endif
