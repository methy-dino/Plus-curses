#pragma once
struct vector2d {
	unsigned x, y;
	vector2d(unsigned x, unsigned y) {
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

