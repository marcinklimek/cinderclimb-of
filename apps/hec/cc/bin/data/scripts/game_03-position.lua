Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")

maxTime = 5*60

game_time = 0

points = 0
area = 0
areaTotal = 0

currentImage = of.Image()

doneSound = of.SoundPlayer()
doneSound:load("sounds/80921__justinbw__buttonchime02up.wav")

positionsAtlas = {}
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())
table.insert(positionsAtlas, of.Image())

positionsAtlas[1]:load("images/pos/image88.png")
positionsAtlas[2]:load("images/pos/image00.png")
positionsAtlas[3]:load("images/pos/image0.png")
positionsAtlas[4]:load("images/pos/image22.png")
positionsAtlas[5]:load("images/pos/image33.png")
positionsAtlas[6]:load("images/pos/image44.png")
positionsAtlas[7]:load("images/pos/image55.png")
positionsAtlas[8]:load("images/pos/image66.png")
positionsAtlas[9]:load("images/pos/image88.png")
positionsAtlas[10]:load("images/pos/image77.png")

positionsAtlasMax = 10
imageIdx = 1

percentFit = 60

currentImage = positionsAtlas[imageIdx]

function setup()

	of.clear(0, 0, 0, 0)

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920, 0) -- (1920 + 1680,0)

end

----------------------------------------------------
function update()

	--uber.getFBO(fbo)
	--currentImageTex:setAlphaMask( fbo:getTexture() )

	if points > percentFit then

		game_time = game_time + 1

		if game_time >= maxTime then

			doneSound:play()

			game_time = 0
			points = 0

			imageIdx = imageIdx + 1
			if imageIdx > positionsAtlasMax then
				imageIdx = 1
			end

			currentImage = positionsAtlas[imageIdx]

		end
	else
		game_time = 0
	end
end


function keyPressed(key)

	if key == 357 then  -- up
		percentFit = percentFit + 1
		if percentFit > 100 then
			percentFit = 100
		end
	end

	if key == 359 then  -- dn

		percentFit = percentFit - 1
		if percentFit < 10 then
			percentFit = 10
		end

	end
end

----------------------------------------------------
function draw()
	of.pushMatrix()
	of.pushStyle()

	of.setColor(255, 255, 255, 255)
	of.scale(0.01, 0.01, 0.01)

	currentImage:draw(0,0,1024,768)
	--fbo:draw(0,0,1024,768)

	of.enableAlphaBlending()
	of.setColor(255, 255, 255, 255)
	of.fill()

	areaTotal = 0
	area = 0
	for i=1, uber.numBlobs do
		local blob = uber.blob(i)

		if blob then

			of.setColor(255, 0, 0, 255)

			of.beginShape()
			of.fill()
			of.enableBlendMode(of.BLENDMODE_ADD)

			for k, v in pairs(blob) do

				of.vertex(v.x, v.y)
				local x = v.x*100
				local y = v.y*100

				if y < currentImage:getHeight() and  x < currentImage:getWidth() and x > 0 and y > 0 then

					local c = currentImage:getColor(x, y)

					areaTotal = areaTotal + 1

					if c:getLightness() > 0 then
						area = area + 1
					end

				end

			end
			of.endShape()
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

	str = "Fit: " .. tostring(points) .. " (" .. tostring(percentFit) .. "%)"
	Utils.drawText( str, 30, 40, true)

	str = "Time: " .. tostring( math.floor((maxTime-game_time)/60) ) .. "s"
	Utils.drawText( str, 820, 40, true)


	of.popMatrix()
end

----------------------------------------------------
function exit()
	print("script finished")
end
