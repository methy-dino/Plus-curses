#pragma once
struct vector2d {
	int x, y;
	vector2d(int x, int y) {
		this->x = x;
		this->y = y;
	}
	bool operator==(const vector2d b){
		return x == b.x && y == b.y;
	}
	bool operator<(const vector2d b) const {
		return x < b.x || y < b.y;
	}
};

struct drawable {
	char* text;
	unsigned width;
};
