uber = UberObj("Kinect")

images = {}

-- create new images

table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())
table.insert(images, of.Image())

images[1]:load("images/start/bz.jpg")
images[2]:load("images/start/bz_1.jpg")
images[3]:load("images/start/bz_2.jpg")
images[4]:load("images/start/bz_3.jpg")
images[5]:load("images/start/bz_4.jpg")
images[6]:load("images/start/bz_5.jpg")
images[7]:load("images/start/bz_6.jpg")
images[8]:load("images/start/bz_7.jpg")
images[9]:load("images/start/bz_8.jpg")
images[10]:load("images/start/bz_9.jpg")
images[11]:load("images/start/bz_10.jpg")
images[12]:load("images/start/bz_11.jpg")

bikeIcon = of.Image()
globalCounter = 0
index = 1
start = 1

----------------------------------------------------
function setup()

	of.setWindowTitle("start")
	of.setFrameRate(60)

	bikeIcon:load("images/start/bike_icon.png")
	bikeIcon:setImageType(of.IMAGE_GRAYSCALE)
end

----------------------------------------------------
function update()

	of.background(0)
	globalCounter = globalCounter + 1

	if globalCounter > 240 then

		globalCounter = 0

		index = index + 1

		if index > 12 then
			index = 1
		end

	end

end

counter = 1

----------------------------------------------------
function draw()

	of.scale(0.01, 0.01, 0.01)

	of.pushMatrix()
	of.setColor(255)

	local scl = 1.1 + math.sin(of.getElapsedTimef()) / 10
	of.scale(scl, scl, scl)

	of.enableAlphaBlending()
	images[index]:draw(0,0)

	of.popMatrix()

	local ofsX = (math.cos(of.getElapsedTimef()) * 200) + 480
	local ofsY = 590

	-- getting the ofColors from an image,
	-- using the brightness to draw circles
	local w = bikeIcon:getWidth()
	local h = bikeIcon:getHeight()
	local diameter = 10
	of.setColor(255, 0, 0)
	for y=1,h-1 do
		for x=1,w-1 do
			local cur = bikeIcon:getColor(x, y)
			local size = 1-(cur:getBrightness() / 256)
			if size * diameter / 2 > 0 then
				of.drawCircle(ofsX + x * diameter, ofsY + y * diameter,
			  	            size * diameter / 2)
			end
		end
	end

	of.scale(100, 100, 100)
		
		for i=1, uber.numBlobs do
		local blob = uber.blob(i)

		if blob then

			of.setColor(128, 128, 128, 255)

			of.beginShape()
			of.fill()
			of.enableBlendMode(of.BLENDMODE_ADD)

			for k, v in pairs(blob) do
				of.vertex(v.x, v.y)
			end
			of.endShape()
		end
	end
		
end

----------------------------------------------------
function exit()
	print("script finished")
end
