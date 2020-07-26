class Color
{
public:

	int r; 
 	int g;
	int b;

	Color()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	Color(int r_, int g_, int b_)
	{
		r = r_;
		g = g_;
		b = b_;
	}


	add(Color other)
	{
		r = r + other.r;
		g = g + other.r;
		b = b + other.r;
	}
}

void main()
{
	Color c_black; // 0,0,0

	c_black.r = 0;
	c_black.g = 1;
	c_black.b = 2;


	Color x(255,255,255);

	x.add( c_black ); // -> x (255, 256, 257)
}