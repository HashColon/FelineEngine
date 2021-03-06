#ifndef HASHCOLON_FELINE_TYPES_VOYAGESIMPLE_HPP
#define HASHCOLON_FELINE_TYPES_VOYAGESIMPLE_HPP

#include <HashColon/Feline/Feline_config.h>
#include <array>
#include <vector>
#include <chrono>
//#include <HashColon/Helper/MultiDimensionArray.h>
#include <HashColon/Helper/ClassPropertyForCpp.hpp>
#include <HashColon/Feline/Types/ValueTypes.hpp>

namespace HashColon::Feline::Types::Simple
{
	using XY = Position;
	using VVa = Velocity;

	namespace hidden
	{
		struct _XYT
		{
			union
			{
				struct
				{
					Types::hidden::_Position _Pos;					
				};
				std::array<HashColon::Helper::Real, 2> dat;
			};
			TimePoint _TP;
			_XYT() : _Pos(), _TP() {};					
		};

		struct _XYVVaT
		{
			union
			{
				struct
				{
					Types::hidden::_Position _Pos;					
					Types::hidden::_Velocity _Vel;
					
				};
				std::array<HashColon::Helper::Real, 4> dat;
			};
			TimePoint _TP;
			_XYVVaT() : _Pos(), _Vel(), _TP() {};
		};
	}

	struct XYT : public hidden::_XYT
	{
		XYT(const XYT& xyt)	: _XYT(xyt) {};		
		XYT(const hidden::_XYT _xyt) : _XYT(_xyt) {};			
		XYT() : _XYT() {};
		XYT(HashColon::Helper::Real _x, HashColon::Helper::Real _y, TimePoint _tp)
		{
			_Pos.longitude = _x;
			_Pos.latitude = _y;
			_TP = _tp;
		};
		XYT(Types::hidden::_Position _pos, TimePoint _tp)
		{
			_Pos = _pos;
			_TP = _tp;
		};

		XYT& operator=(const XYT& rhs) {
			_Pos = rhs._Pos;
			_TP = rhs._TP;
		}

		HashColon::Helper::Real& operator[](std::size_t i) { return dat[i]; }
		TimePoint GetTimePoint()
		{
			return _TP;
		}

		CREATE_PROPERTY(XYT, public, Position, Pos,
			{ return static_cast<Position>(_Pos); },
			{ _Pos = value; }
		);

		CREATE_PROPERTY(XYT, public, TimePoint, TP,
			{ return _TP; },
			{ _TP = value; }
		);

		HashColon::Helper::Real DistanceTo(Types::Position toPoint) const
		{
			Position temp(_Pos);
			return temp.DistanceTo(toPoint);
		};

		HashColon::Helper::Degree AngleTo(Types::Position toPoint) const
		{
			Position temp(_Pos);
			return temp.AngleTo(toPoint);
		};
	};

	// V, Va are velocity of leg before XY
	// ex) wp2 -> leg(2,3) -> wp3 :
	//     for wp3, XYTVVa contains (X3, Y3, T3, V(leg(2,3)), Va(leg(2,3)))
	struct XYVVaT : public hidden::_XYVVaT
	{
		XYVVaT(const XYVVaT& xyvvat) : _XYVVaT(xyvvat) {};
		XYVVaT(const hidden::_XYVVaT _xyvvat) : _XYVVaT(_xyvvat) {};		
		XYVVaT() : _XYVVaT() {};
		XYVVaT(
			HashColon::Helper::Real _x, HashColon::Helper::Real _y, 
			HashColon::Helper::Real _speed, HashColon::Helper::Real _angle,
			TimePoint _tp)
		{
			_Pos.longitude = _x; _Pos.latitude = _y;			
			_Vel.speed = _speed; _Vel.angle = _angle;
			_TP = _tp;
		};

		inline XYVVaT(
			Types::hidden::_Position _pos, 
			Types::hidden::_Velocity _vel,
			TimePoint _tp)
		{
			_Pos = _pos;			
			_Vel = _vel;
			_TP = _tp;
		}

		XYVVaT& operator=(const XYVVaT& rhs)
		{
			_Pos = rhs._Pos;
			_TP = rhs._TP;
			_Vel = rhs._Vel;
		}

		HashColon::Helper::Real& operator[](std::size_t i) { return dat[i]; }
		TimePoint GetTimePoint() const
		{
			return _TP;
		}

		CREATE_PROPERTY(XYVVaT, public, Position, Pos,
			{ return static_cast<Position>(_Pos); },
			{ _Pos = value; }
		);
				
		CREATE_PROPERTY(XYVVaT, public, TimePoint, TP,
			{ return _TP; },
			{ _TP = value; }
		);

		CREATE_PROPERTY(XYVVaT, public, Velocity, Vel,
			{ return static_cast<Velocity>(_Vel); },
			{ _Vel = value; }
		);

		HashColon::Helper::Real DistanceTo(Types::Position toPoint) const
		{
			Position temp(_Pos);
			return temp.DistanceTo(toPoint);
		};

		HashColon::Helper::Degree AngleTo(Types::Position toPoint) const
		{
			Position temp(_Pos);
			return temp.AngleTo(toPoint);
		};
	};

	class XYList : public std::vector<XY>
	{
	public:
		HashColon::Helper::Real GetDistance(int sIndex = 0, int eIndex = -1) const;

		XYList GetNormalizedXYList(int sizeN) const;

		std::vector<HashColon::Helper::Real> GetParameterized() const;

		XYList GetReversed() const;
	};

	class XYTList : public std::vector<XYT>
	{
	public:
		HashColon::Helper::Real GetDistance(int sIndex = 0, int eIndex = -1) const;
		Types::Duration GetDuration(int sIndex = 0, int eIndex = -1) const;		
		XYTList GetReversed() const;
		
		XYList ToXYList() const;
	};

	class XYVVaTList : public std::vector<XYVVaT>
	{
	public:
		HashColon::Helper::Real GetDistance(int sIndex = 0, int eIndex = -1) const;
		Types::Duration GetDuration(int sIndex = 0, int eIndex = -1) const;
		XYVVaTList GetReversed() const;

		XYList ToXYList() const;
		XYTList ToXYTList() const;
	};
}

#endif