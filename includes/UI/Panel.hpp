/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Panel.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:00:12 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 11:40:19 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PANEL_HPP
# define PANEL_HPP

# include "UIElement.hpp"

class	Panel
{
	public:
		Panel() {}
		~Panel()
		{
			for (auto elem : _elements)
				delete elem.second;
		}

		void	clear()
		{
			for (auto elem : _elements)
				delete elem.second;
			_elements.clear();
		}
		void	draw(glm::vec2 windowSize)
		{
			for (auto elem : _elements)
				elem.second->draw(windowSize);
		}
		void	handleEvents(UIEvent events)
		{
			for (auto elem : _elements)
				elem.second->handleEvents(events);
		}
		template <typename T>
		T	*add(const std::string &id, T *element)
		{
			_elements.push_back({id, element});
			return (element);
		}
		UIElement	*get(const std::string &id)
		{
			for (auto elem : _elements)
				if (elem.first == id)
					return (elem.second);
			return (NULL);
		}
	private:
		std::vector<std::pair<std::string, UIElement*>>	_elements;
};

#endif
