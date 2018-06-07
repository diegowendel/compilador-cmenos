void main(void) {
    int idx;
    int values[64];
    int square;
    int row;
    int column;
    int state;

    // Menus especiais do jogo
    int BATTLESHIP_PRESS_TO_ATTACK;
    int BATTLESHIP_SELECT_ROW;
    int BATTLESHIP_SELECT_COLUMN;
    int BATTLESHIP_SHOW_ATTACKED;
    int BATTLESHIP_ALREADY_ATTACKED;

    // Setup
    idx = 0;
    while (idx < 64) {
        values[idx] = 0;
        idx += 1;
    }
    square = 0;
    row = 0;
    column = 0;
    state = 0;

    // Setup menus
    BATTLESHIP_PRESS_TO_ATTACK = 40;
    BATTLESHIP_SELECT_ROW = 41;
    BATTLESHIP_SELECT_COLUMN = 42;
    BATTLESHIP_SHOW_ATTACKED = 43;
    BATTLESHIP_ALREADY_ATTACKED = 44;

    // Loop
    while (1) {
        lcd(BATTLESHIP_PRESS_TO_ATTACK);
        input();
        if (state == 0) {
            lcd(BATTLESHIP_SELECT_ROW);
            row = input();
            if (row >= 1) {
                if (row <= 8) {
                    state = 1;
                }
            }
        }
        
        if (state == 1) {
            lcd(BATTLESHIP_SELECT_COLUMN);
            column = input();
            if (column >= 1) {
                if (column <= 8) {
                    state = 2;
                }
            }
        }
        
        if (state == 2) {
            square = ((row - 1) * 8 ) + column;
            if (values[square - 1] == 0) {
                values[square - 1] = 1;
                state = 3;
            } else {
                state = 0;
                lcd(BATTLESHIP_ALREADY_ATTACKED);
                input();
            }
        }
        
        if (state == 3) {
            state = 0;
            sam(square);
            lcdData(square);
            lcd(BATTLESHIP_SHOW_ATTACKED);
            input();
        }
    }
}
