package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;


public class CardDeck
{
	private static final int NUMBER_OF_CARDS = 13;

	private int size;
	public ArrayList<Card> deck;	//na PRIVATE zmenit

	public CardDeck(int size)
	{
		this.size = size;
		this.deck = new ArrayList<Card>();
	}

	public static CardDeck createStandardDeck()
	{
		CardDeck newDeck = new CardDeck(52);
		for (Card.Color c : Card.Color.values())
		{
			for (int i = 1; i <= NUMBER_OF_CARDS; ++i)
			{
				newDeck.put(new Card(c, i));
			}
		}

		return newDeck;
	}

	public Card pop()
	{
		if (this.deck.size() <= 0)
		{
			return null;
		}

		int lastIndex = this.deck.size() - 1;
		Card tempCard = this.deck.get(lastIndex);
		this.deck.remove(lastIndex);

		return tempCard;
	}

	public void put(Card card)
	{
		if (this.deck.size() >= size)
		{
			return;
		}

		this.deck.add(card);
	}

	public int size()
	{
		return this.deck.size();
	}
}