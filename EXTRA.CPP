
class Pipe {

	public:
	Pipe() {
		x = 0;
		y = 0;
		touched = 0;
		offset = 0;
	}

	int x;
	int y;
	int touched ;
	int offset;
	int width;
	int height;
	int marginTop;
	int marginBottom;

	// will implement in the future
	// will require alot of refactoring elsewhere and will be slow
	draw() {

	}

	update() {


	}

	randomize() {
		offset = rand() % 100;
	}

};

class Birdy {

	public:
	Birdy() {

		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	int x;
	int y;
	int width;
	int height;

	draw() {

	}

	update() {


	}

};

