#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include "Parser.h"

int main()
{
	InitWindow(1280, 1024, "AAA");

	SetTargetFPS(144);

	auto reader = rucoy::ByteStream("maps\\init");
	auto map = rucoy::map_info_from_bytes(reader);
	auto tiles = rucoy::get_map_tiles(map, "maps");

	auto ground_png = LoadTexture("ground.png");

	auto current_layer = 0;

	auto get_source = [&](float x, float y) -> Rectangle
		{
			return Rectangle{ x * map.tile_size, y * map.tile_size, map.tile_size, map.tile_size };
		};

	auto camera = Camera2D{};
	camera.zoom = 0.1f;
	camera.target = Vector2{ 0.f, 1280.f };
	
	while (not WindowShouldClose())
	{
		if (IsKeyPressed(KEY_UP) or IsKeyPressed(KEY_DOWN))
		{
			auto delta = IsKeyPressed(KEY_UP) ? -1 : 1;
			current_layer = std::clamp(current_layer + delta, 0, map.layers - 1);
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			auto delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / camera.zoom);

			camera.target = Vector2Add(camera.target, delta);
		}

		if (auto wheel = GetMouseWheelMove())
		{
			auto mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
			camera.offset = GetMousePosition();
			camera.target = mouseWorldPos;
			camera.zoom += wheel * 0.125f;
			if (camera.zoom < 0.125f)
				camera.zoom = 0.125f;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode2D(camera);
		for (auto x = 0.f; x < map.metrics.width; x++)
			for (auto y = 0.f; y < map.metrics.height; y++)
			{
				for (auto& [texture_x, texture_y] : tiles[current_layer][x + map.metrics.height * ((map.metrics.height - 1) - y)].tiles)
				{
					DrawTextureRec(ground_png, get_source(texture_x, texture_y), Vector2{ x * map.tile_size, y * map.tile_size }, WHITE);
				}
			}

		EndMode2D();


		EndDrawing();
	}

	CloseWindow();
}
