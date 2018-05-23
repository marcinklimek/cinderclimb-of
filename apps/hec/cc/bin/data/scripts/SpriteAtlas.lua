local Sprite = {}

function Sprite.Atlas(r, cyclic)

	local self = {
		time = 1,
		index = 0,
		frames = 0,
		atlas = {},
		rate = r,
		is_cyclic = cyclic,
		is_end = false,
		start = false,
	}

	function self.add(imagePath)
		
		local img = of.Image()
		img:load(imagePath)
		table.insert(self.atlas, img)

		self.frames = self.frames + 1
	end

	function self.finished()
		return is_end
	end
	
	function self.play()
		self.start = true
		self.is_end = false
	end

	function self.update()

		if start == false then
			return
		end

		self.time = self.time + 1

		if (self.time % self.rate) == 0 then
			self.time = 1
			self.index = self.index + 1 

			if self.cyclic == true then
				self.index = self.index % self.frames
			else

				if self.index == self.frames then
					self.start = false
					self.is_end = true
				end

			end
		end
	end

	function self.draw(x, y, w, h)

		if self.start == false then
			return
		end

		self.atlas[self.index+1]:draw(x, y, w, h)

		print (self.index)
	end

    return self
end


return Sprite