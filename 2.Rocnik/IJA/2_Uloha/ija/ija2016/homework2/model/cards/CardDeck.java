package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;


public class CardDeck
{
	private Card.Color onlyColor;
	private ArrayList<Card> deck;

	public CardDeck(Card.Color onlyColor)
	{
		this.deck = new ArrayList<Card>();
		this.onlyColor = onlyColor;
	}

	public Card get()
	{
		if (this.deck.isEmpty())
		{
			return null;
		}

		return this.deck.get(this.deck.size() - 1);
	}

	public Card get(int index)
	{
		if (this.deck.isEmpty())
		{
			return null;
		}

		return this.deck.get(index);
	}

	public boolean isEmpty()
	{
		return this.deck.isEmpty();
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

	public boolean put(Card card)
	{
		if (this.onlyColor != null)
		{
			// Kontrola rovnakej farby balicka
			if (!this.onlyColor.similarColorTo(card.color()))
			{
				return false;
			}

			// Na zaciatok je mozne pridat len eso
			if (this.isEmpty())
			{
				if (card.value() != 1)
				{
					return false;
				}

				this.deck.add(card);
				return true;
			}

			// Rozdiel hodnot kariet musi byt 1
			if (card.value() - this.deck.get(this.size() - 1).value() != 1)
			{
				return false;
			}
		}

		this.deck.add(card);
		return true;
	}

	public int size()
	{
		return this.deck.size();
	}
}
