module ApplicationHelper
	def title(value = nil)
		@title = value if value
		@title ? "#{@title}" : "Weather Dash Board"
	end
end