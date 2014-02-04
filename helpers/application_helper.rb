module ApplicationHelper
	def title(value = nil)
		@title = value if value
		@title ? "#{@title}" : "Wish-A-DJ"
	end
end