#include "funcs.hpp"

void draw_snake (tile tiles[15][16], snake player[240], int length)
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (tiles[i][j].shape.getFillColor() != sf::Color::Red)
                tiles[i][j].shape.setFillColor(sf::Color(85, 163, 97, 255));
            for (int k = 0; k < length; k++)
            {
                if ((player[k].x == j) && (player[k].y == i))
                    tiles[i][j].shape.setFillColor(sf::Color(59, 55, 58, 255));
            }
        }
    }
}

void move_snake (snake player[240], int length, direction dir)
{
    for (int i = length - 1; i > 0; i--)
    {
        if ((player[i].x == player[i].change_x) && (player[i].y == player[i].change_y))
        {
            player[i].change_x = player[i - 1].change_x;
            player[i].change_y = player[i - 1].change_y;
            player[i].dir = player[i - 1].dir;
        }
    }
}

void generate_apple (tile tiles[15][16], snake player[240], int length, apple &a)
{
    vector<int> place_x, place_y;
    int* p;
    int counter = 0;
    
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            bool flag = false;
            for (int k = 0; k < length; k++)
            {
                if ((player[k].x == j) && (player[k].y == i))
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                place_x.push_back(j);
                place_y.push_back(i);
            }
        }
    }
    
    mt19937 gen(time(0));
    uniform_int_distribution<> uid(0, place_x.size() - 1);
    int random = uid(gen);
    a.x = place_x[random];
    a.y = place_y[random];
    tiles[a.y][a.x].shape.setFillColor(sf::Color::Red);
}

bool collision_self (snake player[240], int length)
{
    for (int i = 1; i < length; i++)
    {
        if ((player[0].x == player[i].x) && (player[0].y == player[i].y))
            return true;
    }
    
    return false;
}

bool mouse_on_button (sf::Mouse m, sf::RenderWindow &w, button b)
{
    if ((m.getPosition(w).x > b.body.getPosition().x) && (m.getPosition(w).x < b.body.getPosition().x + b.body.getLocalBounds().width))
    {
        if ((m.getPosition(w).y > b.body.getPosition().y) && (m.getPosition(w).y < b.body.getPosition().y + b.body.getLocalBounds().height))
            return  true;
        else
            return false;
    }
    else
        return false;
}

void init_button (button &b, float pos_x, float pos_y, sf::Color b_color, string text, sf::Font &font, sf::Color t_color, float t_size)
{
    //инициализация текста
    b.text.setFont(font);
    b.text.setString(text);
    b.text.setCharacterSize(t_size);
    b.text.setFillColor(t_color);
    b.text.setPosition(pos_x + b.text.getLocalBounds().height / 2.f, pos_y);
    //инициализация тела
    sf::RectangleShape sh_b(sf::Vector2f(b.text.getLocalBounds().width * 1.045f + b.text.getLocalBounds().height, b.text.getLocalBounds().height * 2));
    b.body = sh_b;
    b.body.setFillColor(b_color);
    b.body.setPosition(pos_x, pos_y);
}

void color_button (button &b, sf::Color b_color, sf::Color t_color)
{
    b.body.setFillColor(b_color);
    b.text.setFillColor(t_color);
}

void init_textbox (textbox &tb, float pos_x, float pos_y, float size_x, float size_y, string text, sf::Font &font, string lab, sf::Font &font_lab, sf::RenderWindow &window)
{
    //инициализация текста
    tb.text.setFont(font);
    tb.text.setString(text);
    tb.text.setCharacterSize(size_y);
    tb.text.setFillColor(sf::Color(0, 0, 0, 128));
    tb.text.setPosition(pos_x + size_x * 0.01f, pos_y);
    //инициализация тела
    sf::RectangleShape sh_b(sf::Vector2f(size_x, tb.text.getLocalBounds().height * 2));
    tb.body = sh_b;
    tb.body.setFillColor(sf::Color(103, 128, 159, 255));
    tb.body.setPosition(pos_x, pos_y);
    //инициализация лейбла
    tb.label.setFont(font_lab);
    tb.label.setString(lab);
    tb.label.setCharacterSize(size_y * 0.8f);
    tb.label.setFillColor(sf::Color(197, 35, 45, 255));
    tb.label.setPosition(pos_x, pos_y - size_y * 1.03);
    //установление вида
    sf::View tb_view(sf::FloatRect(tb.label.getPosition().x, tb.label.getPosition().y, tb.body.getLocalBounds().width, tb.body.getPosition().y + tb.body.getLocalBounds().height- tb.label.getPosition().y));
    tb_view.setViewport(sf::FloatRect((float)tb.label.getPosition().x/window.getSize().x, (float)(tb.label.getPosition().y/window.getSize().y),
                                      (float)tb.body.getLocalBounds().width/window.getSize().x, (float)((tb.body.getPosition().y + tb.body.getLocalBounds().height - tb.label.getPosition().y)/window.getSize().y)));
    tb.v = tb_view;
}

bool mouse_on_textbox (sf::Mouse m, sf::RenderWindow &w, textbox tx)
{
    if ((m.getPosition(w).x > tx.body.getPosition().x) && (m.getPosition(w).x < tx.body.getPosition().x + tx.body.getLocalBounds().width))
    {
        if ((m.getPosition(w).y > tx.body.getPosition().y) && (m.getPosition(w).y < tx.body.getPosition().y + tx.body.getLocalBounds().height))
            return  true;
        else
            return false;
    }
    else
        return false;
}

void name_update (string &name, textbox &tb, sf::Event event)
{
    try
    {
        if ((char)event.text.unicode != '\b')
        {
            tb.text_s.push_back((char)event.text.unicode);
            tb.text.setString(tb.text_s);
            if (tb.text.getLocalBounds().width > tb.body.getLocalBounds().width - tb.body.getSize().x * 0.01f)
                tb.text.setPosition(tb.body.getPosition().x + tb.body.getLocalBounds().width - tb.text.getLocalBounds().width - tb.body.getSize().x * 0.01f, tb.text.getPosition().y);
        }
        else
        {
            if (tb.text_s.length() != 0)
            {
                char del = tb.text_s[tb.text_s.length() - 1];
                tb.text_s.pop_back();
                tb.text.setString(tb.text_s);
                if (tb.text.getLocalBounds().width > tb.body.getLocalBounds().width)
                    tb.text.setPosition(tb.body.getPosition().x + tb.body.getLocalBounds().width - tb.text.getLocalBounds().width - tb.body.getSize().x * 0.01f, tb.text.getPosition().y);
                else
                    tb.text.setPosition(tb.body.getPosition().x + tb.body.getSize().x * 0.01f, tb.text.getPosition().y);
            }
        }
        name = tb.text_s;
    }
    catch (const exception &ex)
    {
        cout << "Вызвано исключение: "<< ex.what();
    }
}

void restart (tile tiles[15][16], snake player[240], FILE *f, sf::RenderWindow &window, sf::RectangleShape &reset_screen, int &apples_collected, apple appl, sf::Text &broke_record, bool &restarted, button &btn, sf::RectangleShape &black_screen)
{
    fseek(f, sizeof(string), SEEK_SET);
    int res;
    fread(&res, sizeof(res), 1, f);
    fseek(f, sizeof(string), SEEK_SET);
    if (apples_collected > res)
    {
        fwrite(&apples_collected, sizeof(apples_collected), 1, f);
        broke_record.setString("New record: " + to_string(apples_collected));
        broke_record.setPosition(404 - broke_record.getLocalBounds().width/2.f, 420);
    }
    else
    {
        broke_record.setString("You haven't broken \n    your record.");
        broke_record.setPosition(404 - broke_record.getLocalBounds().width/2.f, 380);
    }
    
    fseek(f, 0, SEEK_SET);
    for (int i = 0; i < 240; i ++)
    {
        player[i].x = 300;
        player[i].y = 300;
        player[i].change_x = 300;
        player[i].change_y = 300;
    }
    tiles[appl.y][appl.x].shape.setFillColor(sf::Color(85, 163, 97, 255));
    
    reset_screen.setFillColor(sf::Color(7, 41, 12, 255));
    reset_screen.setOutlineColor(sf::Color(32, 84, 40, 255));
    broke_record.setFillColor(sf::Color(255, 0, 0, 255));
    btn.body.setFillColor(sf::Color(65, 128, 32, 255));
    btn.text.setFillColor(sf::Color(168, 30, 30, 255));
    black_screen.setFillColor(sf::Color(0, 0, 0, 200));
    restarted = true;
}

bool mouse_on_triangle (sf::RenderWindow &w, sf::Mouse m, sf::CircleShape tri)
{
    if ((m.getPosition(w).x > tri.getPosition().x) && (m.getPosition(w).x < tri.getPosition().x + tri.getLocalBounds().width))
    {
        if ((m.getPosition(w).y > tri.getPosition().y) && (m.getPosition(w).y < tri.getPosition().y + tri.getLocalBounds().height))
            return  true;
        else
            return false;
    }
    else
        return false;
}

void generate_snake (tile tiles[15][16], snake player[240])
{
    int x = 20, y = 110, color = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            tiles[i][j].x = x;
            tiles[i][j].y = y;
            tiles[i][j].shape.setPosition(x, y);
            tiles[i][j].shape.setSize(sf::Vector2f(45.f, 45.f));
            tiles[i][j].shape.setFillColor(sf::Color(color, color, color, 255));
            x += 48;
            color++;
        }
        x = 20;
        y += 48;
    }
    
    for (int i = 0; i < 240; i++)
    {
        player[i].x = 300;
        player[i].y = 300;
        player[i].change_x = 300;
        player[i].change_y = 300;
    }
    player[0].x = 0;
    player[0].y = 0;
    player[0].dir = d_down;
}
