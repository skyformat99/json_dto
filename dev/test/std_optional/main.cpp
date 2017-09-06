#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <json_dto/pub.hpp>

#if defined( JSON_DTO_SUPPORTS_STD_OPTIONAL )

struct data_t
{
	json_dto::cpp17::optional< int > m_i;
	json_dto::cpp17::optional< float > m_f;
	json_dto::cpp17::optional< std::string > m_s;

	template< typename IO >
	void json_io( IO & io )
	{
		using namespace json_dto;

		io & optional( "int", m_i, json_dto::cpp17::nullopt() )
			& optional( "float", m_f, json_dto::cpp17::nullopt() )
			& optional( "str", m_s, json_dto::cpp17::nullopt() );
	}
};

TEST_CASE( "read from json" , "read" )
{
	{
		const std::string json_data{
			R"JSON(
			{}
			)JSON" };
		auto obj = json_dto::from_json< data_t >( json_data );

		REQUIRE( !obj.m_i );
		REQUIRE( !obj.m_f );
		REQUIRE( !obj.m_s );
	}
	{
		const std::string json_data{
			R"JSON(
			{
				"int":123456
			}
			)JSON" };
		auto obj = json_dto::from_json< data_t >( json_data );

		REQUIRE( obj.m_i );
		REQUIRE( 123456 == *obj.m_i );
		REQUIRE( !obj.m_f );
		REQUIRE( !obj.m_s );
	}
	{
		const std::string json_data{
			R"JSON(
			{
				"float":12.3456,
				"str":"Hello!"
			}
			)JSON" };
		auto obj = json_dto::from_json< data_t >( json_data );

		REQUIRE( !obj.m_i );
		REQUIRE( obj.m_f );
		REQUIRE( Approx(12.3456) == *obj.m_f );
		REQUIRE( obj.m_s );
		REQUIRE( "Hello!" == *obj.m_s );
	}
}

TEST_CASE( "write to json" , "write" )
{
	{
		data_t obj;
		const auto r = json_dto::to_json( obj );

		REQUIRE( R"({})" == r );
	}
	{
		data_t obj;
		obj.m_i = 25;
		const auto r = json_dto::to_json( obj );

		REQUIRE( R"({"int":25})" == r );
	}
	{
		data_t obj;
		obj.m_i = 25;
		obj.m_s = "Bye";
		const auto r = json_dto::to_json( obj );

		REQUIRE( R"({"int":25,"str":"Bye"})" == r );
	}
}

#endif

