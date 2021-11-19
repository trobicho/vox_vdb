/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 08:17:18 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/17 14:23:13 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <thread>
#include <mutex>
#include <list>

#define	THREAD_STATE_SLEEPING		0x1
#define	THREAD_STATE_WORKING		0x2

namespace	utils
{
namespace	thread
{
template	<class Type>
class	event_list
{
	public:
		bool	pop(Type *v)
		{
			std::lock_guard<std::mutex> guard(list_mutex);
			if (!list.empty)
			{
				v = list.front();
				list.pop_front();
				return (true);
			}
			return (false);
		}
		void		push(Type v)
		{
			std::lock_guard<std::mutex> guard(list_mutex);
			list.push_back();
		}
		void		push_front(Type v)
		{
			std::lock_guard<std::mutex> guard(list_mutex);
			list.push_front();
		}
		bool		empty()
		{
			std::lock_guard<std::mutex> guard(list_mutex);
			return (list.empty());
		}
		size_t	size()
		{
			std::lock_guard<std::mutex> guard(list_mutex);
			return (list.size());
		}
		bool		unsafe_empty()
		{
			return (list.empty());
		}
		size_t	unsafe_size()
		{
			return (list.size());
		}

	private:
		std::mutex				list_mutex;
		std::list<Type>		list;
};
}
}

namespace	utils
{
namespace	thread
{
template <class Stats_type>
class	thread_wrapper
{
	public:
		std::thread	thread;
		Stats_type	get_stats() const
		{
			std::lock_guard<std::mutex> guard(mutex);
			return (stats);
		}
		uint32_t		get_state() const
		{
			std::lock_guard<std::mutex> guard(mutex);
			return (state);
		}
		void	set_state(uint32_t a_state)
		{
			std::lock_guard<std::mutex> guard(mutex);
			state = a_state;
		}

	private:
		Stats_type	stats;
		uint32_t		state;
		std::mutex	mutex;
};
}
}
