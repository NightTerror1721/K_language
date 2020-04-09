#pragma once

#include <vcruntime.h>
#include <type_traits>

namespace klang::mem
{
	inline void* malloc(const size_t bytes) { return new unsigned char[bytes]; }
	inline void free(void* ptr) { delete ptr; }



	template<typename _Ty>
	class ResourceView;



	template<typename _Ty>
	class SharedResource
	{
	private:
		_Ty* _data;
		unsigned int* _refs;

	public:
		SharedResource() :
			_data{ nullptr },
			_refs{ nullptr }
		{}
		SharedResource(_Ty* ptr) :
			SharedResource{}
		{
			void* datamem = mem::malloc(sizeof(_data) + sizeof(_refs));
			_data = reinterpret_cast<_Ty*>(datamem);
			_refs = reinterpret_cast<unsigned int*>(_data + 1);
		}
		SharedResource(const SharedResource& sr) :
			_data{ sr._data },
			_refs{ sr._refs }
		{
			if (_data && _refs)
				(*_refs)++;
		}
		SharedResource(SharedResource&& sr) :
			_data{ std::move(sr._data) },
			_refs{ std::move(sr._refs) }
		{
			sr._data = nullptr;
			sr._refs = nullptr;
		}
		~SharedResource()
		{
			if (_data && _refs)
			{
				if (*_refs > 1)
					(*_refs)--;
				else mem::free(_data);

				_data = nullptr;
				_refs = nullptr;
			}
		}

		SharedResource& operator= (const SharedResource& sr)
		{
			if (_data && _refs)
				this->~SharedResource();

			_data = sr._data;
			_refs = sr._refs;
			if(_data && _refs)
				(*_refs)++;

			return *this;
		}

		SharedResource& operator= (SharedResource&& sr)
		{
			if (_data && _refs)
				this->~SharedResource();

			_data = std::move(sr._data);
			_refs = std::move(sr._refs);
			sr._data = nullptr;
			sr._refs = nullptr;

			return *this;
		}

		inline operator bool() const { return _data; }
		inline bool operator! () const { return !_data; }

		_Ty* operator->() { return _data; }
		const _Ty* operator->() const { return _data; }

		friend ResourceView;
	};



	template<typename _Ty>
	class ResourceView
	{
	private:
		_Ty* _ptr;

	public:
		ResourceView() : _ptr{ nullptr } {}
		ResourceView(SharedResource<_Ty>& resource) : _ptr{ resource._data } {}
		ResourceView(const SharedResource<_Ty>& resource) : _ptr{ const_cast<SharedResource<_Ty>&>(resource)._data } {}
		ResourceView(const ResourceView& v) = default;
		ResourceView(ResourceView&& v) = default;

		ResourceView& operator= (const ResourceView&) = default;
		ResourceView& operator= (ResourceView&&) = default;

		inline operator bool() const { return _ptr; }
		inline bool operator! () const { return !_ptr; }

		_Ty* operator->() { return _ptr; }
		const _Ty* operator->() const { return _ptr; }

		template<typename _Dst>
		operator ResourceView<_Dst>()
		{
			static_assert(std::is_base_of<_Dst, _Ty>::value || std::is_base_of<_Ty, _Dst>::value);
			return _ptr ? ResourceView<_Dst>{ reinterpret_cast<_Dst*>(_ptr) } : ResourceView<_Dst>{};
		}

		template<typename _Dst>
		operator const ResourceView<_Dst>() const
		{
			static_assert(std::is_base_of<_Dst, _Ty>::value || std::is_base_of<_Ty, _Dst>::value);
			return _ptr ? ResourceView<_Dst>{ reinterpret_cast<_Dst*>(_ptr) } : ResourceView<_Dst>{};
		}
	};



	template<typename _Ty>
	inline ResourceView<_Ty> view(SharedResource<_Ty>& resource) { return resource; }

	template<typename _Ty>
	inline const ResourceView<_Ty> view(const SharedResource<_Ty>& resource) { return resource; }

	template<typename _Dst, typename _Src>
	inline ResourceView<_Dst> as(ResourceView<_Src>& v) { return static_cast<ResourceView<_Dst>>(v); }

	template<typename _Dst, typename _Src>
	inline const ResourceView<_Dst> as(const ResourceView<_Src>& v) { return static_cast<ResourceView<_Dst>>(v); }
}



namespace klang::heap
{
	/*class Object
	{
	private:
		unsigned int _refs;

	protected:
		constexpr Object() : _refs{ 0 } {}
		virtual ~Object() {}
	};*/


}
