Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

numX = 32
numY = 16
deltaX = 10.24/numX
deltaY = 7.68/numY
border = deltaX*0.1

shapes = {}


local function Shape(x, y, w, h)
    
	    -- public 
	local self = {
		x = 0,
		y = 0,
		w = 1,
		h = 1,
		box = of.Rectangle(x, y, w, h),

		touched = false,
		alpha = 255
	}

	function self.draw()
	    of.pushStyle()	

		of.setColor(255, 255, 255, self.alpha)
		of.drawRectangle( self.box )

		of.popStyle()
	end

	function self.touch()

		self.touched = true

	end

	function self.inside(x, y)

		--local p = of.Point(x, y)
		
		if self.box.inside(x, y) then
		
			self.touched = true

		end

	end

	function self.update()

		if self.touched then

			self.alpha = self.alpha - 10

			if self.alpha < 0 then
				self.alpha = 0
				self.touched = false
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
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

	for x=0,numX-1 do

		for y=0,numY-1 do

			local xx = deltaX * x + border
			local yy = deltaY * y + border
			local ww = deltaX - 2*border
			local hh = deltaY - 2*border

			add_shape(xx, yy, ww, hh)
		end

	end

end

----------------------------------------------------
function update()

	for i=1, uber.numBlobs do

		local blob = uber.blob(i)
		
		if blob then

			of.setColor(255, 0, 0, 128)
			
			for k, v in pairs(blob) do

				local x = v.x / deltaX
				local y = v.y / deltaY

				for i,shape in ipairs(shapes) do
					shape.inside(x, y)
				end


				--local offset = math.floor( x + y*(numX+1) )
				
				--if offset > 0 and offset < numX*numY then
				--	shapes[offset].touch()
				--	print (numX*numY, v.x, v.y, x, y, offset)
				--end
				
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
