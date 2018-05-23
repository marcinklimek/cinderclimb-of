startCounter = 0
globalCounter = 0
game_time = 0

----------------

box2d = require 'LuaBox2D'
uber = UberObj("Kinect")

franklinBook = of.TrueTypeFont()
franklinBook:load("fonts/frabk.ttf", 16)

franklinBookBig = of.TrueTypeFont()
franklinBookBig:load("fonts/frabk.ttf", 32)

bomb = of.Image()
bomb:load("images/gold_1.png")

banner = of.Image()
banner:load("images/banner.jpg")

explosionAtlas = {}

table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())
table.insert(explosionAtlas, of.Image())

explosionAtlas[1]:load("images/explosion_01.png")
explosionAtlas[2]:load("images/explosion_02.png")
explosionAtlas[3]:load("images/explosion_03.png")
explosionAtlas[4]:load("images/explosion_04.png")
explosionAtlas[5]:load("images/explosion_05.png")

touchSound = of.SoundPlayer()
touchSound:load("sounds/balloon.wav")

score = 0
score_ground = 0
level = 1


function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

local function drawText(str, x, y, big)

	of.pushMatrix()
	of.translate(x, y)
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFFFFF)

	if big == true then
		local rect = franklinBookBig:getStringBoundingBox(str, 0, 0);
		-- this is the actual midpt (x + w/2, y + h/2);
		local centerx = rect.x + rect.width / 2
		local centery = rect.y + rect.height / 2		
		franklinBookBig:drawString( str, -centerx, -centery)
	else
		local rect = franklinBookBig:getStringBoundingBox(str, 0, 0);
		-- this is the actual midpt (x + w/2, y + h/2);
		local centerx = rect.x + rect.width / 2
		local centery = rect.y + rect.height / 2		
		franklinBook:drawString( str, -centerx, -centery)
	end

	of.popMatrix()
end


local function Ball(world, _r)
    
    -- public 
	local self = {
		name = "ball",
	    r = _r,
	}

	self.x = of.random(0.2, 9)
	self.y = of.random(-2, -0.5)


	local w = bomb:getWidth()
	local h = bomb:getHeight()
	print(w,h)

	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(self.x, self.y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	body.gravityScale = 0.1

	local circleShape = box2d.CircleShape()
	circleShape.radius = self.r
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = circleShape
	fixtureDef.density = 1
	fixtureDef.friction = 0.3
	fixtureDef.restitution = 0.2

	fixture = body.createFixture(fixtureDef)

	self.counter = 0
	self.state = 0

	body.angularVelocity = of.random(-25, 25)

	function self.draw()
	    
	    of.pushStyle()	
	    of.pushMatrix()
	    of.translate(body.position.x, body.position.y, 0 )
		of.rotate(body.angle, 0, 0, 1)

		local lx = -self.r
		local ly = -self.r
		local lw = 2*self.r
		local lh = 2*self.r

	    	
		of.setColor(255, 255, 255)
		--of.fill()
		--of.drawCircle(body.position.x, body.position.y, self.r)

		if self.state == 0 then
			bomb:draw(lx, ly, lw, lh)
		elseif self.state == 1 then
			explosionAtlas[1]:draw(lx, ly, lw, lh)
		elseif self.state == 2 then
			explosionAtlas[2]:draw(lx, ly, lw, lh)
		elseif self.state == 3 then
			explosionAtlas[3]:draw(lx, ly, lw, lh)
		elseif self.state == 4 then
			explosionAtlas[4]:draw(lx, ly, lw, lh)
		elseif self.state == 5 then
			explosionAtlas[5]:draw(lx, ly, lw, lh)			
		end	

		of.popMatrix()
		of.popStyle()
	end

	function self.update()
		self.counter = self.counter + 1

		if self.state == 0 then

			local x = body.position.x / 10.24
			local y = body.position.y /  7.68

			if uber.insideBlob(x, y) == true then
				self.counter = 0
				self.state = 1

				if game_time > 0 then
					score = score + 1	
				end

			end
		end

		if self.state > 0 then

			if self.counter > 5 then
				self.state = self.state + 1
				self.counter = 0
			end

			if self.state == 6 then
				self.state = 0

				self.x = of.random(0.2, 9)
				self.y = of.random(-1, -0.5)

				body.setTransform( box2d.Vec2(self.x, self.y), 0 )
				body.gravityScale = 0.1 + level/20
				body.linearVelocity = box2d.Vec2(0, 0.1)
				body.angularVelocity = of.random(-15, 15)
				fixture.filter.maskBits = 0x0
			end
		end
	end


	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)

		if obj.name == "ground" then

			-- body.setTransform( box2d.Vec2(self.x, self.y), 0 )
			-- body.linearVelocity = box2d.Vec2(0, 0)
			-- body.angularVelocity = 0
			if self.state == 0 then
				self.counter = 0
				self.state = 1	

				if game_time > 0 then
					score_ground = score_ground + 1
				end
			end
		end

		if obj.name == "player" then

			if self.state == 0 then
				--body.gravityScale = 0.0
				body.linearVelocity = box2d.Vec2(0, 0)
				body.angularVelocity = 0
				fixture.filter.maskBits = 0xffff
				self.counter = 0
				self.state = 1
				--touchSound:play()

				if game_time > 0 then
					score = score + 1
				end
			end
		end

		return true
	end

    return self
end


local function PlayerPoint(world, _x, _y, _r)
    
    -- public 
	local self = {
		name = "player",
	    r = _r,
	    x = _x,
	    y = _y,
	}

	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(self.x, self.y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	body.gravityScale = 0

	local circleShape = box2d.CircleShape()
	circleShape.radius = self.r
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = circleShape
	fixtureDef.density = 1
	fixtureDef.friction = 1
	fixtureDef.restitution = 0.0
	--fixtureDef.sensor = true

	local fixture = body.createFixture(fixtureDef)

	function self.draw()
		of.setColor(255, 130, 0, 50)
		of.fill()
		of.drawCircle(body.position.x, body.position.y, self.r*2)
	end

	function self.setPos(x, y)
		self.x = x
		self.y = y

		body.setTransform( box2d.Vec2(x, y), 0 )
		body.linearVelocity = box2d.Vec2(0, 0)
		body.angularVelocity = 0
	end

	function self.setPosVec(pos)
		self.setPos(pos.x, pos.y)
	end

	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)
	end

    return self
end


local function Player(world)

	-- public
    local self = {
    	name = "ground"
	}


	self.joints = {}
	self.points = {}

	table.insert(self.points, PlayerPoint(world, -100, -10, 0.15) )
	table.insert(self.points, PlayerPoint(world, -100, -20, 0.15) )
	table.insert(self.points, PlayerPoint(world, -100, -30, 0.15) )
	table.insert(self.points, PlayerPoint(world, -100, -40, 0.15) )
	table.insert(self.points, PlayerPoint(world, -100, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -110, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -120, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -130, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -140, -10, 0.15) )
	table.insert(self.points, PlayerPoint(world, -150, -20, 0.15) )
	table.insert(self.points, PlayerPoint(world, -160, -30, 0.15) )
	table.insert(self.points, PlayerPoint(world, -170, -40, 0.15) )
	table.insert(self.points, PlayerPoint(world, -180, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -190, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -200, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -210, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -220, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -230, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -240, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -250, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -260, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -270, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -280, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -290, -55, 0.15) )
	table.insert(self.points, PlayerPoint(world, -290, -65, 0.15) )

	function self.update()

		-- self.joints = {}

		-- local num = uber.numJoints

		-- --if num == 0 then

		-- 	self.points[1].setPos(-100, -10)
		-- 	self.points[2].setPos(-100, -20)
		-- 	self.points[3].setPos(-100, -30)
		-- 	self.points[4].setPos(-100, -40)
		-- 	self.points[5].setPos(-100, -55)
		-- 	self.points[6].setPos(-100, -55)
		-- 	self.points[7].setPos(-100, -65)
		-- 	self.points[8].setPos(-100, -75)
		-- 	self.points[9].setPos(-100, -80)
		-- 	self.points[10].setPos(-100, -90)
		-- 	self.points[11].setPos(-110, -110)
		-- 	self.points[12].setPos(-120, -140)
		-- 	self.points[13].setPos(-130, -155)
		-- 	self.points[14].setPos(-140, -155)
		-- 	self.points[15].setPos(-150, -155)
		-- 	self.points[16].setPos(-160, -155)
		-- 	self.points[17].setPos(-170, -100)
		-- 	self.points[18].setPos(-180, -100)
		-- 	self.points[19].setPos(-190, -140)
		-- 	self.points[20].setPos(-200, -155)
		-- 	self.points[21].setPos(-210, -155)
		-- 	self.points[22].setPos(-220, -155)
		-- 	self.points[23].setPos(-230, -155)			
		-- 	self.points[24].setPos(-240, -155)			
		-- 	self.points[25].setPos(-250, -155)			
		-- --else

		-- 	local numJoints = uber.numJoints
		-- 	for i=1, numJoints do
		-- 		local p = uber.joint(i)
		-- 		self.points[i].setPosVec(p)
		-- 	end	

		-- --end	

	end

	function self.draw()

		of.pushStyle()

		-- of.setColor(0, 250, 0, 50)
		-- of.fill()
		-- for index, value in ipairs(joints) do
		--  	of.drawCircle(value.x, value.y, 0.1)
		-- end

		-- for index, point in ipairs(self.points) do
		-- 	point.draw()
		-- end

		of.enableAlphaBlending()
		of.setColor(0, 200, 50, 90)
		of.noFill()
		for i=1, uber.numBlobs do
			blob = uber.blob(i)
			
			of.beginShape()

			for k, v in pairs(blob) do
				of.vertex(v.x, v.y)
			end

			of.endShape()
		end

		--of.setHexColor(0x000000)
		--of.drawBitmapString(("%i"):format(uber.numJoints), 0.10, 1.50)
		--drawText( ("%i"):format(uber.numJoints), 0.10, 1.50)

		of.popStyle()
	end

	return self
end


local function Scene()

	-- public
    local self = {
    	name = "ground",
    	balls = {}
	}

	-- private
	local timeStep = 1/50
	local velocityIterations = 8
	local positionIterations = 3

	self.balls = {}
	self.world = box2d.World(box2d.Vec2(0, 0.78), true)
	
	local bodyDef = box2d.BodyDef()
	bodyDef.type = 'static'
	bodyDef.position = box2d.Vec2(-100, 7.78)
	bodyDef.gravityScale = 0.0

	local groundBody = self.world.createBody(bodyDef)
	groundBody.userData = self

	local groundBox = box2d.PolygonShape()
	groundBox.setAsBox(200, 0.1)
	
	local groundFixture = groundBody.createFixture(groundBox, 0)

	self.counter = 0

	local function checkContact()
		
		local contact = self.world.contact

		while contact do

			local obj = contact.fixtureA.body.userData
			if obj ~= nil then
				obj.contact(contact.fixtureB.body.userData)
			end

			local obj = contact.fixtureB.body.userData
			if obj ~= nil then
				obj.contact(contact.fixtureA.body.userData)
			end
			
			contact = contact.next
		end
	end


	function self.draw()

	    of.pushMatrix();
	    of.pushStyle()

	    of.translate(groundBody.position.x, groundBody.position.y);

		of.setPolyMode(of.POLY_WINDING_ODD)
		of.setColor(255, 0, 0)
		of.beginShape()
		
		local n = groundBox.vertexCount
		for i=1,n do
			local vtx = groundBox.vertex(i) 
			of.vertex(vtx.x, vtx.y)
		end
			
		of.endShape(true)	

		of.popMatrix()
		of.popStyle()

		for index, ball in ipairs(self.balls) do
			ball.draw()
		end		
	end

	function self.update()

		self.world.step(timeStep, velocityIterations, positionIterations)
		self.world.clearForces()

		for index, ball in ipairs(self.balls) do
			ball.update()
		end		


		if self.counter > 300 then
			self.counter = 0

			if tablelength(self.balls) < 7 then
				table.insert(self.balls, Ball(self.world, of.random(0.2, 0.5)) )
			end
		end

		checkContact()

		self.counter = self.counter + 1
	end

	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)
	end


	function self.gameInit()
		self.balls = {}

		table.insert(self.balls, Ball(self.world, of.random(0.2, 0.5)) )
		table.insert(self.balls, Ball(self.world, of.random(0.2, 0.5)) )
		table.insert(self.balls, Ball(self.world, of.random(0.2, 0.5)) )
	end

	self.gameInit()

	return self
end


-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



function setup()
	print("SETUP\n")
	of.setFrameRate(60)
	of.setWindowTitle("projektor")
	of.setWindowPosition(1920 + 1680,0)
	scene = Scene()
	player = Player(scene.world)
end


----------------------------------------------------

clear_score = 0

function update()
	-- print("UPDATE\n")
	
	if startCounter > 0 then
		startCounter = startCounter - 1
	end

	globalCounter = globalCounter + 1

	if uber.numJoints > 0 then
		
		if clear_score == 1 then
			
			level = 0
			score = 0
			score_ground = 0

			clear_score = 0
		end

		scene.update()
		player.update()

		if globalCounter > 60 then
			globalCounter = 0
			
			game_time = game_time - 1

			if game_time < 0 then
				game_time = 0
			end

			if game_time % 5 == 0 then
				level = level + 1
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

	if uber.numJoints > 0 then
		


		player.draw()
		scene.draw()
	else
		drawText("BRAK GRACZY",5,4,true)




	end


	of.pushMatrix()
	of.scale(0.01, 0.01, 0.01)
	of.setHexColor(0xFFFFFF)
	
	str = "Punkty: " .. tostring(score) .. "/" .. tostring(score_ground)
	franklinBookBig:drawString( str, 30, 690)  

	str = "Poziom: " .. tostring(level) 
	franklinBook:drawString( str, 32, 650)  

	str = "Czas: " .. tostring(game_time) .. "s"
	franklinBookBig:drawString( str, 820, 690)  

	of.popMatrix()

end

function drawUI()

	-- if startCounter > 0 then
	-- 	of.disableAlphaBlending()
	-- 	of.setHexColor(0x808080)
	-- 	banner:draw(0, 0, 1024, 768)
	-- 	of.enableAlphaBlending()
	-- end



end

----------------------------------------------------
function exit()
	print("box2d script finished")
end
