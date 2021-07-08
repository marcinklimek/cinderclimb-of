banner = of.Image()
banner:load("images/banner.jpg")

explosionAtlas = {}

table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())

explosionAtlas[1]:load("images/bz.jpg")
explosionAtlas[2]:load("images/bz_1.jpg")
explosionAtlas[3]:load("images/bz_2.jpg")
explosionAtlas[4]:load("images/bz_3.jpg")
explosionAtlas[5]:load("images/bz_4.jpg")
explosionAtlas[6]:load("images/bz_5.jpg")
explosionAtlas[7]:load("images/bz_6.jpg")
explosionAtlas[8]:load("images/bz_7.jpg")
explosionAtlas[9]:load("images/bz_8.jpg")
explosionAtlas[10]:load("images/bz_9.jpg")
explosionAtlas[11]:load("images/bz_10.jpg")
explosionAtlas[12]:load("images/bz_11.jpg")

-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

globalCounter = 0
index = 1
start = 1

function setup()
	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	--of.setWindowPosition(1920,0)
end


----------------------------------------------------
function update()
	-- print("UPDATE\n")

	globalCounter = globalCounter + 1
end


----------------------------------------------------
function draw()

	of.pushMatrix()
	of.scale(0.01, 0.01, 0.01)

	if start == 1 then
		banner:draw(0, 0, 1024, 768)
		
		if globalCounter > 240 then
			index = 1	
			start = 0
			globalCounter = 0
		end

	else

		if globalCounter > 240 then
			globalCounter = 0
			
			index = index + 1
			if index > 12 then
				index = 1
			end
		end

		explosionAtlas[index]:draw(0, 0, 1024, 768)
	end

	of.popMatrix()
end

----------------------------------------------------
function exit()
end
