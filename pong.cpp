#include "raylib.h"
#include <iostream>
#include <string>

class Player {
public:
    int x, y;
    int width, height;
    int speed;
    Player(int x = 0, int y = 0, int w = 0, int h = 0, int speed = 0) : x(x), y(y), width(w), height(h), speed(speed) {}

    int hitboxU{y};
    int hitboxL{x};
    int hitboxD{y + height};
    int hitboxR{x + width};

    void move() {
        y += speed;

        hitboxU = y;
        hitboxL = x;
        hitboxD = y + height;
        hitboxR = x + width;
    }

    bool collisionU() {
        return (hitboxU < 0);
    }
    bool collisionD(int height) {
        return (hitboxD > height);
    }
};

class Ball {
public:
    int x, y;
    int radius;
    int speedX;
    int speedY;

    Ball(int x = 0, int y = 0, int radius = 0, int speed = 0) : x(x), y(y), radius(radius), speedX(speed), speedY(speed) {}
    
    int hitboxU{y - radius};
    int hitboxL{x - radius};
    int hitboxD{y + radius};
    int hitboxR{x + radius};

    void move() {
        x += speedX;
        y += speedY;

        hitboxU = y - radius;
        hitboxL = x - radius;
        hitboxD = y + radius;
        hitboxR = x + radius;
    }

    bool collisionX(Player player) {
        return (player.hitboxL <= hitboxR && player.hitboxR >= hitboxL);
    }
    bool collisionY(Player player) {
        return (player.hitboxU <= hitboxD && player.hitboxD >= hitboxU);
    }
    
    bool collisionX(int width, int height) {
        return (hitboxL < 0 || hitboxR > width);
    }
    bool collisionY(int width, int height) {
        return (hitboxU < 0 || hitboxD > height);
    }
};

int main() {
    int width{800}, height{600};
    InitWindow(width, height, "pong");

    Ball ball(width/2, height/2, 25, 0);
    Player player1(10, height / 2 - 65, 15, 130, 0);
    Player player2(width - 25, height / 2 - 65, 15, 130, 0);

    int points1{}, points2{};
    bool cheat_mode = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{10, 10, 10, 255});
        DrawLine(width / 2, 0, width / 2, height, WHITE);
        DrawCircleLines(width / 2, height / 2, 35, Color{255, 255, 255, 150});

        if (IsKeyDown(KEY_UP) && IsKeyUp(KEY_DOWN)) {
            if (player1.speed == 0)
                player1.speed = -10;
            if (player1.speed > 0)
                player1.speed *= -1;
            
            if (ball.speedX == 0 || ball.speedY == 0) {
                ball.speedX = 9;
                ball.speedY = 9;
            }
            if (player2.speed == 0)
                player2.speed = 7;
        }
        if (IsKeyDown(KEY_DOWN) && IsKeyUp(KEY_UP)) {
            if (player1.speed == 0)
                player1.speed = 10;
            if (player1.speed < 0)
                player1.speed *= -1;

            if (ball.speedX == 0 || ball.speedY == 0) {
                ball.speedX = 9;
                ball.speedY = 9;
            }
            if (player2.speed == 0)
                player2.speed = 7;
        }
        if (IsKeyPressed(KEY_LEFT_SHIFT)) {
            if (cheat_mode)
                cheat_mode = false;
            else
                cheat_mode = true;
        }

        if (player1.collisionU())
            player1.y = 0;
        if (player1.collisionD(height))
            player1.y = height - player1.height;
        
        int diff = width / 9 * (7.43 + (float)GetRandomValue(0, 13) / 100);

        if (cheat_mode) {
            DrawRectangle(diff, 0, width, height, Color{230, 40, 60, 20});
            DrawLine(diff, 0, diff, height, Color{230, 40, 60, 100});
            DrawText("Detecting\nZone", width / 9 * 7.5 + 5, height / 2 - 10, 20, Color{230, 40, 60, 50});
        }

        if (ball.x > diff) {
            if (player2.hitboxU > ball.y && player2.speed > 0)
                player2.speed *= -1;
            if (player2.hitboxD < ball.y && player2.speed < 0)
                player2.speed *= -1;
        }

        if (player2.collisionU())
            player2.y = 0;
        if (player2.collisionD(height))
            player2.y = height - player2.height;
            
        player1.move();
        player2.move();

        DrawRectangleLines(player1.x, player1.y, player1.width, player1.height, Color{0, 220, 230, 255});
        DrawRectangle(player1.x + 2, player1.y + 2, player1.width - 4, player1.height - 4, Color{0, 220, 230, 255});
        DrawRectangleLines(player2.x, player2.y, player2.width, player2.height, Color{200, 50, 60, 255});
        DrawRectangle(player2.x + 2, player2.y + 2, player2.width - 4, player2.height - 4, Color{200, 50, 60, 255});
        
        if (ball.collisionX(player1) && ball.collisionY(player1)) {
            ball.speedX *= -1;
            ball.x = player1.hitboxR + ball.radius;
        }
        if (ball.collisionX(player2) && ball.collisionY(player2)) {
            ball.speedX *= -1;
            ball.x = player2.hitboxL - ball.radius;
        }
        if (ball.collisionX(width, height)) {
            if (ball.hitboxL <= 0)
                points2++;
            if (ball.hitboxR >= width)
                points1++;

            ball.x = width / 2;
            ball.y = height / 2;
            player1.speed = 0;
            player2.speed = 0;
            ball.speedX = 0;
            ball.speedY = 0;
            ball.move();

            player1.x = 10;
            player1.y = height / 2 - player1.height / 2;
            player1.move();
            player2.x = width - 25;
            player2.y = height / 2 - player2.height / 2;
            player2.move();

            WaitTime(0.4);
        }
        if (ball.collisionY(width, height))
            ball.speedY *= -1;
        
        DrawCircleLines(ball.x, ball.y, ball.radius, WHITE);
        DrawCircle(ball.x, ball.y, ball.radius - 2, WHITE);
        ball.move();
        
        if (cheat_mode) {
            if (ball.speedX < 0 && ball.speedY != 0) {
                int k = 0;
                while (ball.x - k * abs(ball.speedX) > ball.radius)
                    k++;
                int predct;
                
                if (ball.speedY < 0)
                    predct = ball.y - k * abs(ball.speedX);
                else
                    predct = ball.y + k * abs(ball.speedX);

                if (predct > height) {
                    DrawCircleLines(ball.radius, height - predct + height, ball.radius, Color{230, 40, 60, 100});
                    DrawCircle(ball.radius, height - predct + height, ball.radius - 2, Color{230, 40, 60, 25});
                }
                else {
                    DrawCircleLines(ball.radius, abs(predct), ball.radius, Color{230, 40, 60, 100});
                    DrawCircle(ball.radius, abs(predct), ball.radius - 2, Color{230, 40, 60, 25});
                }
            }
            
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (ball.speedX == 0 || ball.speedY == 0) {
                    ball.speedX = 9;
                    ball.speedY = 9;
                }
                if (player2.speed == 0)
                    player2.speed = 7;
                
                player1.speed = 0;
                player1.y = GetMouseY() - player1.height / 2;
                player1.move();
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                ball.x = GetMouseX();
                ball.y = GetMouseY();
            }
        }

        DrawText(std::to_string(points1).c_str(), width / 4 + 2, height / 15 + 2, 20, Color{0, 220, 230, 200});
        DrawText(std::to_string(points1).c_str(), width / 4, height / 15, 20, WHITE);
        DrawText(std::to_string(points2).c_str(), width - width / 4 - 2, height / 15 + 2, 20, Color{200, 50, 60, 200});
        DrawText(std::to_string(points2).c_str(), width - width / 4, height / 15, 20, WHITE);

        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();
        EndDrawing();
    }
    CloseWindow();
}