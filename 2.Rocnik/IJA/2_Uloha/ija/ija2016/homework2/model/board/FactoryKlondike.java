package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.*;


public class FactoryKlondike extends AbstractFactorySolitaire
{
	private static final int NUMBER_OF_CARDS = 13;

	@Override
	public Card createCard(Card.Color color, int value)
	{
		if (value >= 14 || value <= 0)
		{
			return null;
		}

		return new Card(color, value);
	}

	@Override
	public CardDeck createCardDeck()
	{
		CardDeck newDeck = new CardDeck(null);
		for (Card.Color c : Card.Color.values())
		{
			for (int i = 1; i <= NUMBER_OF_CARDS; ++i)
			{
				newDeck.put(new Card(c, i));
			}
		}

		return newDeck;
	}

	@Override
	public CardDeck createTargetPack(Card.Color color)
	{
		return new CardDeck(color);
	}

	@Override
	public CardStack createWorkingPack()
	{
		return new CardStack(true);
	}
}
