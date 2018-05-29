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
		return self.is_end
	end
	
	function self.play()
		self.start = true
		self.is_end = false
	end

	function self.update()

		if self.start == true then

			self.time = self.time + 1

			if (self.time % self.rate) == 0 then
				self.time = 1
				self.index = self.index + 1 

				if self.cyclic == true then
					self.index = self.index % self.frames
				else
					--print ("sprite end", self.index, self.frames, self.index >= self.frames)
					if self.index >= self.frames then
						self.start = false
						self.is_end = true
					end
				end
			end
		end
	end

	function self.draw(x, y, w, h)

		if self.start == true then
			--print(self.index, (self.index % self.frames) + 1)
			self.atlas[(self.index % self.frames) + 1]:draw(x, y, w, h)
		end
	end

    return self
end


return Sprite