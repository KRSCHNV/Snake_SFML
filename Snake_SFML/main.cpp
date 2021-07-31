#include "funcs.hpp"

int load_game (sf::RenderWindow &window, FILE* f, sf::Mouse mouse, sf::Font Otomanopee, sf::Font QlassikBold)
{
    //ИНТЕРФЕС
    //фигуры
    sf::RectangleShape background_interface, border1(sf::Vector2f(3, 150)), border2(sf::Vector2f(3, 150)), reset_screen, black_screen;
    background_interface.setSize(sf::Vector2f(808.f, 90));
    background_interface.setPosition(0, 0);
    background_interface.setFillColor(sf::Color(7, 41, 12, 255));
    background_interface.setOutlineThickness(-3.f);
    background_interface.setOutlineColor(sf::Color(32, 84, 40, 255));
    
    border1.setPosition(228, 0);
    border1.setFillColor(sf::Color(32, 84, 40, 255));
    
    border2.setPosition(515, 0);
    border2.setFillColor(sf::Color(32, 84, 40, 255));
    
    reset_screen.setSize(sf::Vector2f(400.f, 400.f));
    reset_screen.setPosition(204, 250);
    reset_screen.setFillColor(sf::Color(7, 41, 12, 0));
    reset_screen.setOutlineThickness(-3.f);
    reset_screen.setOutlineColor(sf::Color(32, 84, 40, 0));
    
    black_screen.setSize(sf::Vector2f(765, 717));
    black_screen.setPosition(20, 110);
    black_screen.setFillColor(sf::Color(0, 0, 0, 0));
    
    //Текст
    sf::Text apples_text, apples_collected_text, bs_text, name_text, broke_record;
    int apples_collected = 0;
    apples_text.setString("Apples: " + to_string(apples_collected));
    apples_text.setCharacterSize(30);
    apples_text.setFillColor(sf::Color::Red);
    apples_text.setFont(Otomanopee);
    apples_text.setPosition(114 - apples_text.getLocalBounds().width/2.f, 25);
    
    int best_result;
    bs_text.setCharacterSize(30);
    bs_text.setFillColor(sf::Color::Red);
    bs_text.setFont(Otomanopee);
    
    string name;
    name_text.setCharacterSize(40);
    name_text.setFont(Otomanopee);
    name_text.setFillColor(sf::Color::Red);
    
    broke_record.setCharacterSize(35);
    broke_record.setFont(Otomanopee);
    broke_record.setFillColor(sf::Color(255, 0, 0, 0));
    
    //Чтение из файла
    int difficulty, sleep_d;
    fread(&name, sizeof(name), 1, f);
    name_text.setString(name);
    name_text.setPosition(404 - name_text.getLocalBounds().width/2.f, 840);
    fread(&best_result, sizeof(best_result), 1, f);
    bs_text.setString("Best score: " + to_string(best_result));
    bs_text.setPosition(373 - bs_text.getLocalBounds().width/2.f, 25);
    fread(&difficulty, sizeof(difficulty), 1, f);
    switch (difficulty) {
        case 0:
            sleep_d = 130000;
            break;
        case 1:
            sleep_d = 90000;
            break;
        case 2:
            sleep_d = 60000;
            break;
        default:
            break;
    }
    fseek(f, 0, SEEK_SET);
    
    //Кнопки
    button pause, exit, restart_btn;
    init_button(pause, 558, 25, sf::Color(65, 128, 32, 255), "Pause", QlassikBold, sf::Color(168, 30, 30, 255), 26);
    init_button(exit, 698, 25, sf::Color(65, 128, 32, 255), "Exit", QlassikBold, sf::Color(168, 30, 30, 255), 26);
    init_button(restart_btn, 404, 500, sf::Color(65, 128, 32, 0), "Restart", QlassikBold, sf::Color(168, 30, 30, 0), 30);
    restart_btn.body.move(-restart_btn.body.getLocalBounds().width/2.f, 0);
    restart_btn.text.move(-restart_btn.body.getLocalBounds().width/2.f, 0);
    pause.body.setOutlineThickness(4);
    pause.body.setOutlineColor(sf::Color(0, 0, 0, 0));
    
    //ГЕЙМПЛЕЙ
    //Создание змеи
    int snake_length = 1;
    bool moved = true, restarted = false, paused = false;;
    tile tiles[15][16];
    direction dir = d_down;
    snake player[240];
    generate_snake(tiles, player);
    
    //Создание яблока
    apple appl;
    appl.x = 0;
    appl.y = 0;
    generate_apple(tiles, player, snake_length, appl);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.TextEntered && !paused)
            {
                switch (event.text.unicode) {
                    case 97: //a
                        if ((player[0].dir != d_right) && moved)
                            dir = d_left;
                        break;
                    case 119: //w
                        if ((player[0].dir != d_down) && moved)
                            dir = d_up;
                        break;
                    case 100: //d
                        if ((player[0].dir != d_left) && moved)
                            dir = d_right;
                        break;
                    case 115: //s
                        if ((player[0].dir != d_up) && moved)
                            dir = d_down;
                        break;
                    default:
                        break;
                }
                
                moved = false;
                player[0].dir = dir;
                int counter = 0;
                for (int i = 1; i < snake_length; i++)
                {
                    if (player[i].change_x == 300)
                    {
                        player[i].change_x = player[0].x;
                        player[i].change_y = player[0].y;
                    }
                }
            }
            if (event.type == event.MouseButtonPressed)
            {
                if (mouse_on_button(mouse, window, exit))
                    return 0;
                if (mouse_on_button(mouse, window, pause))
                {
                    !paused ? pause.body.setOutlineColor(sf::Color(255, 0, 0, 255)) : pause.body.setOutlineColor(sf::Color(255, 0, 0, 0));
                    paused = !paused;
                }
                if (mouse_on_button(mouse, window, restart_btn))
                    return 1;
            }
        }
        
        mouse_on_button(mouse, window, pause) ? color_button(pause, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(pause, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        mouse_on_button(mouse, window, exit) ? color_button(exit, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(exit, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        if (restarted)
            mouse_on_button(mouse, window, restart_btn) ? color_button(restart_btn, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(restart_btn, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        
        if ((player[0].x == appl.x) && (player[0].y == appl.y)) //проверка на столкновение с яблоком
        {
            snake_length++;
            apples_collected++;
            apples_text.setString("Apples: " + to_string(apples_collected));
            apples_text.setPosition(114 - apples_text.getLocalBounds().width/2.f, 25);
            switch (player[snake_length - 2].dir) {
                case d_up:
                    player[snake_length - 1].x = player[snake_length - 2].x;
                    player[snake_length - 1].y = player[snake_length - 2].y + 1;
                    player[snake_length - 1].dir = d_up;
                    break;
                case d_right:
                    player[snake_length - 1].x = player[snake_length - 2].x - 1;
                    player[snake_length - 1].y = player[snake_length - 2].y;
                    player[snake_length - 1].dir = d_right;
                    break;
                case d_down:
                    player[snake_length - 1].x = player[snake_length - 2].x;
                    player[snake_length - 1].y = player[snake_length - 2].y - 1;
                    player[snake_length - 1].dir = d_down;
                    break;
                case d_left:
                    player[snake_length - 1].x = player[snake_length - 2].x + 1;
                    player[snake_length - 1].y = player[snake_length - 2].y;
                    player[snake_length - 1].dir = d_left;
                    break;
                default:
                    break;
            }
            player[snake_length - 1].change_x = player[snake_length - 2].change_x;
            player[snake_length - 1].change_y = player[snake_length - 2].change_y;
            generate_apple(tiles, player, snake_length, appl);
        }
        
        if (snake_length < 240 && !paused)
        {
            for (int i = 0; i < snake_length; i++)
            {
                switch (player[i].dir)
                {
                    case d_up:
                            player[i].y -= 1;
                        break;
                    case d_right:
                            player[i].x += 1;
                        break;
                    case d_down:
                            player[i].y += 1;
                        break;
                    case d_left:
                            player[i].x -= 1;
                        break;
                    default:
                        break;
                }
            }
            if (snake_length > 3) //проверка на столкновение с самим собой
                if (collision_self(player, snake_length) && !restarted)
                    restart(tiles, player, f, window, reset_screen, apples_collected, appl, broke_record, restarted, restart_btn, black_screen);
            if ((((player[0].x < 0) || (player[0].x > 15)) || ((player[0].y < 0) || (player[0].y > 14))) && !restarted) //проверка на столкновение со стенками
                restart(tiles, player, f, window, reset_screen, apples_collected, appl, broke_record, restarted, restart_btn, black_screen);
            
            move_snake(player, snake_length, dir);
        }
        
        draw_snake(tiles, player, 240);
        window.clear(sf::Color(32, 84, 40, 255));
        window.setView(window.getDefaultView());
        window.draw(background_interface);
        window.draw(apples_text);
        window.draw(bs_text);
        window.draw(border1);
        window.draw(border2);
        window.draw(name_text);
        window.draw(pause.body);
        window.draw(pause.text);
        window.draw(exit.body);
        window.draw(exit.text);
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                window.draw(tiles[i][j].shape);
            }
        }
        window.draw(black_screen);
        window.draw(reset_screen);
        window.draw(broke_record);
        window.draw(restart_btn.body);
        window.draw(restart_btn.text);
        window.display();
        moved = true;
        usleep(sleep_d);
    }
}

void enter_options (sf::RenderWindow &window, sf::Font &text_font, sf::Color text_color, sf::Mouse mouse, FILE* f, sf::Font Otomanopee, sf::Font QlassikBold)
{
    //Текст
    sf::Text difficulty_tx, easy_tx, medium_tx, high_tx;
    difficulty_tx.setFont(text_font);
    difficulty_tx.setCharacterSize(40);
    difficulty_tx.setString("DIFFICULTY: ");
    difficulty_tx.setFillColor(text_color);
    difficulty_tx.setPosition(20, 20);
    
    //Фигуры
    sf::CircleShape triangle_11(10, 3), triangle_12(10, 3);
    triangle_11.setOrigin(10, 10);
    triangle_11.rotate(-90);
    triangle_11.setPosition(50 + difficulty_tx.getLocalBounds().width, 20 + difficulty_tx.getLocalBounds().height);
    
    triangle_12.setOrigin(10, 10);
    triangle_12.rotate(90);
    int dif_cur = 0;
    
    //Текст
    easy_tx.setFont(text_font);
    easy_tx.setCharacterSize(40);
    easy_tx.setString("EASY");
    easy_tx.setFillColor(text_color);
    easy_tx.setPosition(triangle_11.getPosition().x + triangle_11.getLocalBounds().width, 20);
    
    medium_tx.setFont(text_font);
    medium_tx.setCharacterSize(40);
    medium_tx.setString("MEDIUM");
    medium_tx.setFillColor(text_color);
    medium_tx.setPosition(triangle_11.getPosition().x + triangle_11.getLocalBounds().width, 20);
    
    high_tx.setFont(text_font);
    high_tx.setCharacterSize(40);
    high_tx.setString("HIGH");
    high_tx.setFillColor(text_color);
    high_tx.setPosition(triangle_11.getPosition().x + triangle_11.getLocalBounds().width, 20);
    sf::Text difficulty_arr[] = {easy_tx, medium_tx, high_tx};
    
    //Кнопки
    button apply_btn;
    init_button(apply_btn, 630, 815, sf::Color(65, 128, 32, 255), "APPLY", text_font, sf::Color(168, 30, 30, 255), 35);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
           if (event.type == event.MouseButtonPressed)
           {
               if (mouse_on_triangle(window, mouse, triangle_11))
               {
                   if (dif_cur > 0)
                       dif_cur--;
                   else
                       dif_cur = 2;
               }
               else if (mouse_on_triangle(window, mouse, triangle_12))
               {
                   if (dif_cur < 2)
                       dif_cur++;
                   else
                       dif_cur = 0;
               }
               
               if (mouse_on_button(mouse, window, apply_btn))
               {
                   fseek(f, sizeof(string), SEEK_SET);
                   fseek(f, sizeof(int), SEEK_CUR);
                   fwrite(&dif_cur, sizeof(dif_cur), 1, f);
                   fseek(f, 0, SEEK_SET);
                   return;
               }
           }
        }
        mouse_on_triangle(window, mouse, triangle_11) ? triangle_11.setFillColor(sf::Color::Red) : triangle_11.setFillColor(sf::Color::White);
        mouse_on_triangle(window, mouse, triangle_12) ? triangle_12.setFillColor(sf::Color::Red) : triangle_12.setFillColor(sf::Color::White);
        mouse_on_button(mouse, window, apply_btn) ? color_button(apply_btn, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(apply_btn, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        
        window.clear(sf::Color(31, 71, 9, 255));
        window.setView(window.getDefaultView());
        window.draw(difficulty_tx);
        window.draw(triangle_11);
        window.draw(difficulty_arr[dif_cur]);
        triangle_12.setPosition(difficulty_arr[dif_cur].getPosition().x + difficulty_arr[dif_cur].getLocalBounds().width + difficulty_arr[dif_cur].getLocalBounds().width * 0.15f, 20 + difficulty_tx.getLocalBounds().height);
        window.draw(triangle_12);
        window.draw(apply_btn.body);
        window.draw(apply_btn.text);
        window.display();
    }
    
}

void enter_name (sf::RenderWindow &window, sf::Font &text_font, sf::Color text_color, sf::Mouse mouse, FILE* f, sf::Font Otomanopee, sf::Font QlassikBold)
{
    //Курсоры
    sf::Cursor cursor_text, cursor_arrow;
    cursor_text.loadFromSystem(sf::Cursor::Text);
    cursor_arrow.loadFromSystem(sf::Cursor::Arrow);
    
    //Текст
    sf::Text warning_tx;
    warning_tx.setFont(text_font);
    warning_tx.setCharacterSize(20);
    warning_tx.setString("Warning: enter data!");
    warning_tx.setFillColor(sf::Color(197, 35, 45, 0));
    warning_tx.setPosition(166, 440);
    
    //Текстбоксы
    textbox ent_name_tb;
    init_textbox(ent_name_tb, 166, 380, 350, 40, "Enter text here...", QlassikBold, "Enter your name:", text_font, window);
    string name;
    
    //Кнопки
    button go_btn, back_btn;
    init_button(go_btn, 536, 380, sf::Color(65, 128, 32, 255), "GO!", text_font, sf::Color(168, 30, 30, 255), 43);
    init_button(back_btn, 670, 780, sf::Color(197, 45, 45, 255), "BACK", text_font, sf::Color(255, 255, 255, 255), 30);
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.MouseButtonPressed)
            {
                if (mouse_on_textbox(mouse, window, ent_name_tb))
                {
                    if (!ent_name_tb.text_s.size())
                        ent_name_tb.text.setString("");

                    ent_name_tb.body.setFillColor(sf::Color(103, 128, 193, 255));
                    ent_name_tb.text.setFillColor(sf::Color(56, 2, 2, 255));
                    ent_name_tb.clicked = true;
                }
                else
                {
                    if (!ent_name_tb.text_s.size() && ent_name_tb.clicked)
                    {
                        ent_name_tb.text.setString("Enter text...");
                    }

                    ent_name_tb.body.setFillColor(sf::Color(103, 128, 159, 255));
                    ent_name_tb.text.setFillColor(sf::Color(0, 0, 0, 128));
                    ent_name_tb.clicked = false;
                }
                
                if (mouse_on_button(mouse, window, back_btn))
                    return;
                
                if (mouse_on_button(mouse, window, go_btn))
                {
                    if (!name.length())
                        warning_tx.setFillColor(sf::Color(197, 35, 45, 255));
                    else
                    {
                        int score = 0, dif = 1;
                        fwrite(&name, sizeof(name), 1, f);
                        fwrite(&score, sizeof(score), 1, f);
                        fwrite(&dif, sizeof(dif), 1, f);
                        fseek(f, 0, SEEK_SET);
                        while (load_game(window, f, mouse, Otomanopee, QlassikBold))
                            cout << "restart..." << endl;
                        return;
                    }
                }
            }
            if (event.type == event.TextEntered)
                name_update(name, ent_name_tb, event);
            
            
        }
        
        mouse_on_button(mouse, window, go_btn) ? color_button(go_btn, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(go_btn, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        mouse_on_button(mouse, window, back_btn) ? color_button(back_btn, sf::Color(197, 45, 45, 255), sf::Color(255, 255, 255, 255)) : color_button(back_btn, sf::Color(197, 45, 45, 128), sf::Color(255, 255, 255, 128));
        mouse_on_textbox(mouse, window, ent_name_tb) ? window.setMouseCursor(cursor_text) : window.setMouseCursor(cursor_arrow);
        
        window.clear(sf::Color(31, 71, 9, 255));
        window.setView(window.getDefaultView());
        window.draw(go_btn.body);
        window.draw(go_btn.text);
        window.draw(back_btn.body);
        window.draw(back_btn.text);
        window.draw(warning_tx);
        window.setView(ent_name_tb.v);
        window.draw(ent_name_tb.body);
        window.draw(ent_name_tb.label);
        window.draw(ent_name_tb.text);
        window.display();
    }
}

int main(int, char const**)
{
    //Настройка окна
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(808, 900), "Snake", sf::Style::Close | sf::Style::Titlebar, settings);
    window.setFramerateLimit(60);
    
    //Загрузка шрифтов
    sf::Font Abril, Otomanopee, QlassikBold;
    sf::Mouse mouse;
    Abril.loadFromFile("/Users/vladimirkorsunov/Desktop/Programming/SFML_learn/Snake_SFML/Snake_SFML/AbrilFatface-Regular.ttf");
    Otomanopee.loadFromFile("/Users/vladimirkorsunov/Desktop/Programming/SFML_learn/Snake_SFML/Snake_SFML/OtomanopeeOne-Regular.ttf");
    QlassikBold.loadFromFile("/Users/vladimirkorsunov/Desktop/Programming/SFML_learn/Snake_SFML/Snake_SFML/QlassikBold_TB.otf");
    
    //Кнопки
    button game, options, exit;
    init_button(game, 404, 300, sf::Color(65, 128, 32, 255), "GAME", Abril, sf::Color(168, 30, 30, 255), 40);
    game.body.move(-game.body.getLocalBounds().width/2.f, 0);
    game.text.move(-game.body.getLocalBounds().width/2.f, 0);
    init_button(options, 404, 380, sf::Color(65, 128, 32, 255), "OPTIONS", Abril, sf::Color(168, 30, 30, 255), 40);
    options.body.move(-options.body.getLocalBounds().width/2.f, 0);
    options.text.move(-options.body.getLocalBounds().width/2.f, 0);
    init_button(exit, 404, 460, sf::Color(65, 128, 32, 255), "EXIT", Abril, sf::Color(168, 30, 30, 255), 40);
    exit.body.move(-exit.body.getLocalBounds().width/2.f, 0);
    exit.text.move(-exit.body.getLocalBounds().width/2.f, 0);
    
    //Бинарный файл для хранения имени, лучшего результата и настроек
    FILE *f = fopen("/Users/vladimirkorsunov/Desktop/Programming/SFML_learn/Snake_SFML/Snake_SFML/player_data.bin", "r+b");
    if (f == NULL)
    {
        cout << "Ошибка, файл не может быть открыт.";
        return;
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.MouseButtonPressed)
            {
                if (mouse_on_button(mouse, window, exit))
                {
                    fclose(f);
                    window.close();
                }
                else if (mouse_on_button(mouse, window, game))
                {
                    string name;
                    fread(&name, sizeof(name), 1, f);
                    fseek(f, 0, SEEK_SET);
                    if (!name.length())
                        enter_name(window, Abril, sf::Color(168, 30, 30, 255), mouse, f, Otomanopee, QlassikBold);
                    else
                    {
                        while (load_game(window, f, mouse, Otomanopee, QlassikBold))
                            cout << "restart..." << endl;
                    }
                }
                else if (mouse_on_button(mouse, window, options))
                    enter_options(window, Abril, sf::Color(168, 30, 30, 255), mouse, f, Otomanopee, QlassikBold);
                    
            }
        }
        mouse_on_button(mouse, window, game) ? color_button(game, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(game, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        mouse_on_button(mouse, window, options) ? color_button(options, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(options, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        mouse_on_button(mouse, window, exit) ? color_button(exit, sf::Color(168, 30, 30, 255), sf::Color(65, 128, 32, 255)) : color_button(exit, sf::Color(65, 128, 32, 255), sf::Color(168, 30, 30, 255));
        
        window.clear(sf::Color(31, 71, 9, 255));
        window.setView(window.getDefaultView());
        window.draw(game.body);
        window.draw(game.text);
        window.draw(options.body);
        window.draw(options.text);
        window.draw(exit.body);
        window.draw(exit.text);
        window.display();
    }
    
    return 0;
}

