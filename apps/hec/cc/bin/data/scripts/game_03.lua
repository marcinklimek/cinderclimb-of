Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

game_time = 0

local function Player(world)

	-- public
    local self = {
    	name = "ground",
    	fbo = of.Image()
   	}

	self.fbo:allocate(1024, 768, of.IMAGE_COLOR_ALPHA);

	function self.update()
		uber.getFBO(self.fbo)
	end

	function self.drawFBO()

		of.enableAlphaBlending()
		of.setColor(250, 250, 250, 250)
		--of.noFill()
		of.fill()

		for i=1, uber.numBlobs do
			local blob = uber.blob(i)
			
			if blob then
				of.beginShape()
				
				for k, v in pairs(blob) do
					of.vertex(v.x, v.y)
				end

				of.endShape()
			end
		end
	end

	function self.draw()
		of.enableBlendMode(of.BLENDMODE_ADD);
		self.fbo:draw(0,0)
	end
	

	return self
end

function game_update()

	player.update()
end

-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

function setup()
	
	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

	player = Player()
end


----------------------------------------------------

function update()
	
	if uber.numBodies > 0 then
		game_update()

		game_time = game_time + 1

	end

end

function drawFBO()
	of.setColor( 0,0,0,2 )
	of.drawRectangle( of.getWindowRect() )
    player.drawFBO()
end


function keyPressed(key)
end

----------------------------------------------------
function draw()

	player.draw()

	of.pushStyle()
	of.pushMatrix()
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFAFFF)
	
	str = "Czas: " .. tostring(game_time)
	Utils.drawText( str, 820, 690, true)  

	of.popMatrix()
	of.popStyle()
end

----------------------------------------------------
function exit()
	print("script finished")
end
