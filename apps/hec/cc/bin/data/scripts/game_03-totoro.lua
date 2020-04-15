Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

game_time = 0

totoros = {}

table.insert(totoros, "images/totoro/bz_1.jpg" )
table.insert(totoros, "images/totoro/bz_10.jpg" )
table.insert(totoros, "images/totoro/bz_11.jpg" )
table.insert(totoros, "images/totoro/bz_2.jpg" )
table.insert(totoros, "images/totoro/bz_3.jpg" )
table.insert(totoros, "images/totoro/bz_4.jpg" )
table.insert(totoros, "images/totoro/bz_5.jpg" )
table.insert(totoros, "images/totoro/bz_6.jpg" )
table.insert(totoros, "images/totoro/bz_7.jpg" )
table.insert(totoros, "images/totoro/bz_8.jpg" )
table.insert(totoros, "images/totoro/bz_9.jpg" )
table.insert(totoros, "images/My-Neighbour-Totoro-scale.jpg" )
table.insert(totoros, "images/totoro/bz_0.jpg" )

totoro = of.Image()
idx = math.floor( of.random(1, 13) )
print (idx)
print( totoros[idx] )
totoro:load( totoros[idx] )
totoroTex = totoro:getTexture()

fbo = of.Image()
fbo:allocate(1024, 768, of.IMAGE_COLOR_ALPHA);

print("-------------------------------------------------------------------------------------------------------------------------------")

function setup()
	
	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowPosition(1920, 0) -- (1920 + 1680,0)

end

----------------------------------------------------

function update()
	
	uber.getFBO(fbo)
	totoroTex:setAlphaMask( fbo:getTexture() )

end

function drawFBO()
	
	of.enableAlphaBlending()

	if uber.numBlobs == 0 then

		--of.setColor( 0,0,0,255 )
		--of.drawRectangle( of.getWindowRect() )

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

	of.pushStyle()
	of.pushMatrix()
	
	
	of.scale(0.01, 0.01, 0.01)
	of.enableBlendMode(of.BLENDMODE_ADD)

	totoro:draw(0,0,1024,768)
	of.popMatrix()

    of.pushMatrix()

	of.fill()

	of.enableBlendMode(of.BLENDMODE_ALPHA)
	of.setColor(255, 0, 0, 250)
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
	
	of.popMatrix()

	of.popStyle()
end

----------------------------------------------------
function exit()
	print("script finished")
end
