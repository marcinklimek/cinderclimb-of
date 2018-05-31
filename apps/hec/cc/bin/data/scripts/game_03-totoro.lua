Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

game_time = 0

totoro = of.Image()
totoro:load("images/My-Neighbour-Totoro.jpg")
totoroTex = totoro:getTexture()

fbo = of.Image()
fbo:allocate(1024, 768, of.IMAGE_COLOR_ALPHA);

function setup()
	
	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

end

----------------------------------------------------

function update()
	
	uber.getFBO(fbo)
	totoroTex:setAlphaMask( fbo:getTexture() )

end

function drawFBO()
	
	of.enableAlphaBlending()

	if uber.numBlobs == 0 then

		of.setColor( 0,0,0,255 )
		of.drawRectangle( of.getWindowRect() )

	else

		-- fading
		--of.setColor( 0,0,0,2 )
		--of.drawRectangle( of.getWindowRect() )

		of.enableBlendMode(of.BLENDMODE_ADD)
		of.setColor(255, 255, 255, 80)
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
end


function keyPressed(key)
end

----------------------------------------------------
function draw()
	of.pushMatrix()
	of.pushStyle()
	
	of.scale(0.01, 0.01, 0.01)
	of.enableBlendMode(of.BLENDMODE_ADD)

	totoro:draw(0,0,1024,768)
	
	of.popStyle()
	of.popMatrix()
end

----------------------------------------------------
function exit()
	print("script finished")
end
