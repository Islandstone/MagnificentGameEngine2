
#include "physics.h"
#include "script.h"

class ChainWorldBuilder
{
public:
	ChainWorldBuilder()
	{
		m_vertices.clear();
	}

	void AddVertex(float x, float y)
	{
		m_vertices.push_back( b2Vec2(x, y) );
	}

	// TODO: Make an overload of this that accepts a body def
	void Finish()
	{
		b2ChainShape chain;
		b2BodyDef worlddef;
		b2Body *ground;

		b2Vec2 *a = new b2Vec2[ m_vertices.size() ];
		std::copy( m_vertices.begin(), m_vertices.end(), a );

		chain.CreateLoop(a, m_vertices.size());
		worlddef.position.Set(0,0);
		Physics()->CreateBody( &chain, &worlddef );
	}

private:
	std::vector<b2Vec2> m_vertices;

};

void BindChainWorldBuilder()
{
	Sqrat::Class<ChainWorldBuilder> def;

	def.Func(L"AddVertex", &ChainWorldBuilder::AddVertex);
	def.Func(L"Finish", &ChainWorldBuilder::Finish);

	Sqrat::RootTable().Bind(L"ChainWorldBuilder", def);
}

ClassBinder chainWorldBinder(&BindChainWorldBuilder);