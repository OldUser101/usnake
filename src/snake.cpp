#include "snake.h"

namespace usnake {
    void Snake::init_cells() {
        SNAKE_CELL c = {};
        c.x = 1;
        c.y = 1;

        direction = RIGHT;

        snake_cells.clear();
        snake_cells.push_back(c);
    }

    void Snake::handle_food(SNAKE_CELL s) {
        if (s.x == ap.x && s.y == ap.y) {
            _score++;
            set_food_pos();

            SNAKE_CELL n = {};
            SNAKE_CELL l = snake_cells[snake_cells.size() - 1];

            n.x = l.x;
            n.y = l.y;
            n.attributes = l.attributes;

            snake_cells.push_back(n);
        }
    }

    bool Snake::point_over_snake(int x, int y) {
        for (int i = 0; i < snake_cells.size(); i++) {
            if (snake_cells[i].x == x && snake_cells[i].y == y) {
                return true;
            }
        }
        return false;
    }

    void Snake::set_food_pos() {
        int x;
        int y;
        
        do {
            x = rand() % WIDTH;
            y = rand() % HEIGHT;
        } while (point_over_snake(x, y));

        ap.x = x;
        ap.y = y;
    }

    void Snake::new_game() {
        timer->stop();
        init_cells();
        set_food_pos();
        timer->start();
        _score = 0;
        _gameover = false;
    }

    void Snake::pause_game() {
        timer->stop();
        _isPaused = true; 
    }

    void Snake::resume_game() {
        timer->start();
        _isPaused = false;
    }

    void Snake::cycle() {
        std::vector<SNAKE_CELL> newSnake;

        for (int i = 0; i < snake_cells.size(); i++)
        {
            if (i == 0)
            {
                SNAKE_CELL s = snake_cells[0];

                int newX = s.x;
                int newY = s.y;
                int newDirection = direction;

                if (directionPending) {
                    newDirection = pendingDirection;
                    directionPending = false;
                }

                switch (newDirection) {
                    case UP:
                        newY--;
                        break;
                    case RIGHT:
                        newX++;
                        break;
                    case DOWN:
                        newY++;
                        break;
                    case LEFT:
                        newX--;
                        break;
                }

                newX = newX < 0 ? (WIDTH + newX) : newX;
                newY = newY < 0 ? (HEIGHT + newY) : newY;

                newX %= WIDTH;
                newY %= HEIGHT;

                if (point_over_snake(newX, newY)) {
                    timer->stop();
                    _gameover = true;
                }

                SNAKE_CELL n = {};
                n.x = newX;
                n.y = newY;
                n.attributes = s.attributes;

                direction = newDirection;

                newSnake.push_back(n);

                handle_food(n);
            } else {
                SNAKE_CELL s = snake_cells[i];
                SNAKE_CELL ps = snake_cells[i - 1];

                SNAKE_CELL n = {};
                n.x = ps.x;
                n.y = ps.y;
                n.attributes = s.attributes;

                newSnake.push_back(n);
            }
        }

        snake_cells.clear();
        
        for (int i = 0; i < newSnake.size(); i++) {
            snake_cells.push_back(newSnake[i]);
        }
    }

    void Snake::set_direction(int dir) {
        int axis = (direction == UP || direction == DOWN) ? 0 : 1;

        if ((axis == 1 && (dir == UP || dir == DOWN)) || (axis == 0 && (dir == RIGHT || dir == LEFT)))
        {
            pendingDirection = dir;
            directionPending = true;
        }
    }

    bool Snake::isPaused() const {
        return _isPaused;
    }

    bool Snake::gameover() const {
        return _gameover;
    }

    int Snake::score() const {
        return _score;
    }

    ATTRIBUTED_POINT Snake::food() const {
        return ap;
    }

    Snake::Snake(Timer *timer) {
        this->timer = timer;
        new_game();
    }

    Snake::Snake() {
        return;
    }
}