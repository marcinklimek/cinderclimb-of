Utils = require "Utils"

uber = UberObj("Kinect")

numX = 32
numY = 16
deltaX = 10.24/numX
deltaY = 7.68/numY
border = deltaX*0.1

shapes = {}

colorTimer = 0
colorR = 255
colorG = 255
colorB = 255

local function Shape(x, y, w, h)

	    -- public
	local self = {
		x = 0,
		y = 0,
		w = 1,
		h = 1,
		x2 = x + w,
		y2 = y + h,
		box = of.Rectangle(x, y, w, h),
		timer = 0,
		touched = false,
		back = false,
		alpha = 255,
		r = 255,
		g = 255,
		b = 255
	}

	function self.draw()
	    of.pushStyle()

		of.setColor(self.r, self.g, self.b, self.alpha)
		of.drawRectangle( self.box )

		of.popStyle()
	end

	function self.touch()

		self.touched = true

	end

	function self.new_color()


	end


	function self.inside(x, y)

		if self.touched or self.back then
			return
		end

		--print (x >= self.x , y >= self.y , x <= self.x2 , y <= self.y2)
		--local p = of.Point(x, y)

		if x >= self.x and y >= self.y and x <= self.x2 and y <= self.y2 then

			self.touched = true

		end

		return self.touched
	end

	function self.update()

		if self.touched then

			self.alpha = self.alpha - 4

			if self.alpha < 0 then
				self.alpha = 0
				self.touched = false
				self.wait = true
			end
		elseif self.wait then
			self.timer = self.timer + 1

			if self.timer > 30 then
				self.timer = 0
				self.wait = false
				self.back = true

				self.r = colorR
				self.g = colorG
				self.b = colorB

			end
		elseif self.back then
			self.alpha = self.alpha + 10

			if self.alpha > 255 then
				self.alpha = 255
				self.touched = false
				self.back = false
			end
		end

	end

    return self
end


function add_shape(x,y,w,h)
	--print(x,y,w,h)
	table.insert(shapes, Shape(x,y,w,h))
end


function setup()

	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 0) -- (1920 + 1680,0)

	for y=0,numY-1 do
		for x=0,numX-1 do

			local xx = deltaX * x + border
			local yy = deltaY * y + border
			local ww = deltaX - 2*border
			local hh = deltaY - 2*border

			add_shape(xx, yy, ww, hh)
		end

	end

end

function round(x)
  return x>=0 and math.floor(x+0.5) or math.ceil(x-0.5)
end

----------------------------------------------------
function update()

	colorTimer = colorTimer + 1

	if colorTimer == 120 then

		colorTimer = 0

		colorR = of.random(100, 255)
		colorG = of.random(100, 255)
		colorB = of.random(100, 255)
	end

	for i=1, uber.numBlobs do

		local blob = uber.blob(i)

		if blob then

			of.setColor(255, 0, 0, 128)

			for k, v in pairs(blob) do

				local x = round(v.x / deltaX)
				local y = round(v.y / deltaY)

				--for i,shape in ipairs(shapes) do
				--	shape.inside(x, y)
				--end
				local offset = round( x + y*(numX) )
				--print(v.x, v.y, x, y, offset)


				if x < numX and y < numY then
					shapes[offset].touch()
					--print (numX*numY, v.x, v.y, x, y, offset)
				end

			end

		end
	end

	for i,shape in ipairs(shapes) do
		shape.update()
	end

end


function keyPressed(key)
end

----------------------------------------------------
function draw()
	of.pushMatrix()
	of.pushStyle()

	of.enableAlphaBlending()
	of.setColor(255, 255, 255, 255)

	of.fill()

	for i,shape in ipairs(shapes) do
		shape.draw()
	end


	for i=1, uber.numBlobs do
		local blob = uber.blob(i)

		if blob then

			of.setColor(255, 0, 0, 128)
			of.beginShape()

			for k, v in pairs(blob) do
				of.vertex(v.x, v.y)
			end

			of.endShape()
		end
	end


	of.popStyle()
	of.popMatrix()
end

----------------------------------------------------
function exit()
	print("script finished")
end
