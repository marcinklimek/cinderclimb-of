Sprite = require "SpriteAtlas"
box2d = require "LuaBox2D"
uber = UberObj("Kinect")

startCounter = 0
globalCounter = 0
game_time = 0

----------------
franklinBook = of.TrueTypeFont()
franklinBook:load("fonts/frabk.ttf", 16)

franklinBookBig = of.TrueTypeFont()
franklinBookBig:load("fonts/frabk.ttf", 32)

touchSound = of.SoundPlayer()
touchSound:load("sounds/balloon.wav")



score = 0
score_ground = 0
level = 10


function tablelength(T)
	local count = 0
	
	for _ in pairs(T) do 
		count = count + 1 
	end
	
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


local function player_add_score(value)

	if game_time > 0 then
		score = score + 1	
	end	

end

local function ground_add_score(value)

	if game_time > 0 then
		score_ground = score_ground + 1	
	end	

end

function generate_pos(ball, bounds)
	local path = of.random(0, 1)
	
	if path > 0.5 then
		ball.x = of.random(0, bounds.min_x) * 10.24
		ball.x = ball.x + 2*ball.r
	else
		ball.x = of.random(bounds.max_x, 1) * 10.24
		ball.x = ball.x - 2*ball.r
	end

	ball.y = of.random(-2, -0.5)		
end

local function Ball(world, bounds)
    
	    -- public 
	local self = {
		name = "ball",
	    r = of.random(0.1, 0.6),
		state = 0,
		x = 0,
		y = 0
	}

	self.ballAnim = Sprite.Atlas(5, false)
	self.ballAnim.add("images/explosion_01.png")
	self.ballAnim.add("images/explosion_02.png")
	self.ballAnim.add("images/explosion_03.png")
	self.ballAnim.add("images/explosion_04.png")
	self.ballAnim.add("images/explosion_05.png")

	self.bomb = of.Image()
	self.bomb:load("images/gold_1.png")

	generate_pos(self, bounds)

	local bodyDef = box2d.BodyDef()
	bodyDef.position = box2d.Vec2(self.x, self.y)
	bodyDef.type = 'dynamic'

	local body = world.createBody(bodyDef)
	body.userData = self
	body.gravityScale = 0.1 * level

	local circleShape = box2d.CircleShape()
	circleShape.radius = self.r
	
	local fixtureDef = box2d.FixtureDef()
	fixtureDef.shape = circleShape
	fixtureDef.density = 1
	fixtureDef.friction = 0.2
	fixtureDef.restitution = 0.2

	fixture = body.createFixture(fixtureDef)

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

		if self.state == 0 then
			self.bomb:draw(lx, ly, lw, lh)
		else
			self.ballAnim.draw(lx, ly, lw, lh)
		end

		of.popMatrix()
		of.popStyle()
	end

	function self.update(bounds)

		local x = body.position.x / 10.24
		local y = body.position.y /  7.68

		--print("update", self.state, self.ballAnim.finished(), x, y)
		if self.state == 0 then


			local r = 2*self.r

			if self.y > 2.0 then
				self.state = 1
			end

			if uber.insideBlob(x, y, self.r/10.24) == true then

				self.state = 1

				player_add_score(1)

			end

		elseif self.state == 1 then

			self.ballAnim.play()
			self.state = 2
		else

			self.ballAnim.update()

			if self.ballAnim.finished() == true then

				self.state = 0

				generate_pos(self, bounds)

				body.setTransform( box2d.Vec2(self.x, self.y), 0 )
				body.gravityScale = 0.1 + level/20
				body.linearVelocity = box2d.Vec2(0, 0.1)
				body.angularVelocity = of.random(-25, 25)
			end

		end
	end


	function self.contact(obj)

		if obj.name == "ground" then

			if self.state == 0 then
				self.state = 1
				ground_add_score(1)
			end
		end

		return true
	end

    return self
end


local function Player(world)

	-- public
    local self = {
    	name = "ground"
   	}

	function self.update()
	end

	function self.draw()

		of.enableAlphaBlending()
		of.setColor(0, 200, 50, 90)
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

	end

	return self
end


local function Scene()

	-- public
    local self = {
    	name = "ground",
    	balls = {},
    	blob_rect = {min_x = 0.1, max_x = 0.2, min_y = 0.1, max_y = 0.2}
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

	function self.updateBoundingBox(rect)
	
		if self.blob_rect.min_x > rect.min_x then
			self.blob_rect.min_x = rect.min_x
		end

		if self.blob_rect.min_y > rect.min_y then
			self.blob_rect.min_y = rect.min_y
		end

		if self.blob_rect.max_x < rect.max_x  then
			self.blob_rect.max_x = rect.max_x
		end

		if self.blob_rect.max_y < rect.max_y then
			self.blob_rect.max_y = rect.max_y
		end	


		if self.blob_rect.min_x < 0 then
			self.blob_rect.min_x = 0
		end

		if self.blob_rect.min_y < 0 then
			self.blob_rect.min_y = 0
		end

		if self.blob_rect.max_x > 1 then
			self.blob_rect.max_x = 1
		end

		if self.blob_rect.max_y > 1 then
			self.blob_rect.max_y = 1
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

		for i=1, uber.numBlobs do
			self.updateBoundingBox(uber.blobMinMax(i))
		end

		self.world.step(timeStep, velocityIterations, positionIterations)
		self.world.clearForces()

		for index, ball in ipairs(self.balls) do
			ball.update( self.blob_rect )
		end		

		if self.counter > 100 then
			self.counter = 0
			self.addBall()
		end

		checkContact()

		self.counter = self.counter + 1
	end

	function self.contact(obj)
		-- print("contact ", self.name, "->", obj.name)
	end


	function self.gameInit()
		self.balls = {}

		self.addBall()
	end

	function self.addBall()

		if tablelength(self.balls) < 7 then
			table.insert(self.balls, Ball(self.world, self.blob_rect) )	
		end

	end

	self.gameInit()

	return self
end


function update_game()
	player.update()
	scene.update()
end

-- --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



function setup()
	
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

	if uber.numBodies > 0 then
		
		if clear_score == 1 then
			
			level = 1
			score = 0
			score_ground = 0

			clear_score = 0
		end

		update_game()

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
