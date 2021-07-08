Utils = require "Utils"

box2d = require "LuaBox2D"
uber = UberObj("Kinect")


startCounter = 0
globalCounter = 0
game_time = 0
clear_score = 0

score = 0
score_ground = 0
level = 10

--#define GL_RGBA32F_ARB 0x8814
--#define GL_RGB32F_ARB 0x8815
--#define GL_RGB 0x1907
--#define GL_RGBA 0x1908

fbo = of.Fbo()
fbo:allocate(1024, 768, 0x8814)
print("fbo allocated: "..tostring(fbo:isAllocated()))
print("fbo - "..tostring(fbo:getWidth()).." "..tostring(fbo:getHeight()))

local function Player(world)

	-- public
    local self = {
    	name = "ground"
   	}

	function self.update()
		fbo:beginFbo()
		
		of.pushStyle()

		of.setLineWidth(1)
		of.disableAlphaBlending()
		of.setColor(150, 150, 150, 255)
		of.noFill()

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

		of.popStyle()
		fbo:endFbo()		
	end

	function self.draw()

		-- of.enableAlphaBlending()
		-- of.setColor(0, 200, 50, 90)
		-- of.noFill()

		-- for i=1, uber.numBlobs do
		-- 	local blob = uber.blob(i)
			
		-- 	if blob then
		-- 		of.beginShape()
				
		-- 		for k, v in pairs(blob) do
		-- 			of.vertex(v.x, v.y)
		-- 		end

		-- 		of.endShape()
		-- 	end
		-- end

		local txture = fbo:getTexture()
		txture:draw(0, 0)

	end

	return self
end

function game_update()
	player.update()
end

-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

function setup()
	
	fbo:beginFbo()
	of.clear(0, 0, 0, 0)
	fbo:endFbo()

	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 , 10) -- (1920 + 1680,0)

	player = Player()
end


----------------------------------------------------

function update()
	
	if startCounter > 0 then
		startCounter = startCounter - 1
	end

	globalCounter = globalCounter + 1

	if uber.numBodies > 0 then
		
		if clear_score == 1 then
			
			level = 1
			score = 0
			score_ground = 0

			clear_score = 0
		end

		game_update()

		if globalCounter > 60 then
			globalCounter = 0
			
			game_time = game_time - 1

			if game_time < 0 then
				game_time = 0
			end

			if game_time % 20 == 0 then
				level = level + 1
			end

			if level > 40 then
				level = 40
			end

		end
	
	else   -- no player

		game_time = 60

		clear_score = 1
	end

end

function keyPressed(key)
	if key == 357 then  -- up
		level = level + 1
		if level > 40 then
			level = 40
		end		
	end

	if key == 359 then  -- dn

		level = level - 1
		if level < 0 then
			level = 0
		end

	end	
end

----------------------------------------------------
function draw()
	-- print("DRAW\n")

	-- gra
	if startCounter > 0 then
		return
	end

	player.draw()

	-- if uber.numBodies > 0 then
	-- 	player.draw()
	-- else
	-- 	Utils.drawTextCentered("BRAK GRACZY", 5, 4, true)
	-- end

	of.pushMatrix()
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFFFFF)
	
	str = "Punkty: " .. tostring(score) .. "/" .. tostring(score_ground)
	Utils.drawText( str, 30, 690, true)

	str = "Poziom: " .. tostring(level) 
	Utils.drawText( str, 32, 650, false)  

	str = "Czas: " .. tostring(game_time) .. "s"
	Utils.drawText( str, 820, 690, true)  

	of.popMatrix()
end

----------------------------------------------------
function exit()
	print("script finished")
end
