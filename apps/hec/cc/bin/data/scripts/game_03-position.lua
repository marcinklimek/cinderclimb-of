Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

maxTime = 5*60

game_time = 0

points = 0
area = 0
areaTotal = 0

totoro = of.Image()

explosionAtlas = {}
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())

explosionAtlas[1]:load("images/pos/image0-glow.png")
explosionAtlas[2]:load("images/pos/image2.png")
explosionAtlas[3]:load("images/pos/image3.png")
explosionAtlas[4]:load("images/pos/image4.png")
explosionAtlas[5]:load("images/pos/image5.png")
explosionAtlas[6]:load("images/pos/image1.png")
imageIdx = 1


totoro = explosionAtlas[imageIdx]

function setup()
	
	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

end

----------------------------------------------------
function update()
	
	--uber.getFBO(fbo)
	--totoroTex:setAlphaMask( fbo:getTexture() )

	if points > 50 then

		game_time = game_time + 1

		if game_time > maxTime then
			
			game_time = 0
			points = 0

			imageIdx = imageIdx + 1
			if imageIdx > 6 then
				imageIdx = 1
			end

			totoro = explosionAtlas[imageIdx]

		end
	else
		game_time = 0
	end

end


function keyPressed(key)
end

----------------------------------------------------
function draw()
	of.pushMatrix()
	of.pushStyle()
	
	of.setColor(255, 255, 255, 255)
	of.scale(0.01, 0.01, 0.01)

	totoro:draw(0,0,1024,768)
	--fbo:draw(0,0,1024,768)

	of.enableAlphaBlending()
	of.setColor(255, 255, 255, 255)
	of.fill()

	-- for i=1, uber.numBlobs do
	-- 	local blob = uber.blob(i)
		
	-- 	if blob then

	-- 		of.setColor(255, 0, 0, 128)
	-- 		of.beginShape()
			
	-- 		for k, v in pairs(blob) do
	-- 			--of.vertex(v.x, v.y)
	-- 			of.vertex(v.x*100, v.y*100)
	-- 		end

	-- 		of.endShape()
	-- 	end
	-- end

	areaTotal = 0
	area = 0
	for i=1, uber.numBlobs do
		local blob = uber.blob(i)
		
		if blob then

			of.setColor(255, 0, 0, 255)
			--of.beginShape()
			
			for k, v in pairs(blob) do
				--of.vertex(v.x, v.y)
				local x = v.x*100
				local y = v.y*100

				if y > totoro:getHeight() then
					y = 0
				end

				if x > totoro:getWidth() then
					x = 0
				end

				local c = totoro:getColor(x, y)
				of.drawCircle(x, y, 5)

				areaTotal = areaTotal + 1

				if c:getLightness() > 0 then
					
										
					area = area + 1
				end
			end

			--of.endShape()
		end
	end

	if areaTotal == 0 then
		areaTotal = 1
	end
	points = math.floor(area/areaTotal * 100 + 0.5)

	of.popStyle()
	of.popMatrix()

	of.pushMatrix()
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFFFFF)
	
	str = "Dopasowanie: " .. tostring(points)
	Utils.drawText( str, 30, 690, true)

	str = "Czas: " .. tostring( math.floor((maxTime-game_time)/60) ) .. "s"
	Utils.drawText( str, 820, 690, true) 


	of.popMatrix()	
end

----------------------------------------------------
function exit()
	print("script finished")
end