package ija.ija2016.homework2.model.cards;

import java.lang.IllegalArgumentException;


public class Card
{
	private Card.Color color;
	private int value;
	private boolean faceUp;

	public Card(Card.Color c, int value)
	{
		this.color = c;
		this.value = value;
		this.faceUp = false;
	}

	public static enum Color
	{
		CLUBS, DIAMONDS, HEARTS, SPADES;

		@Override
		public String toString()
		{
			switch(this)
			{
				case CLUBS: return "C";
				case DIAMONDS: return "D";
				case HEARTS: return "H";
				case SPADES: return "S";
				default: throw new IllegalArgumentException();
			}
		}

		public boolean similarColorTo(Card.Color c)
		{
			if ((this == CLUBS && c == SPADES) || (this == SPADES && c == CLUBS))
			{
				return true;
			}

			if ((this == DIAMONDS && c == HEARTS) || (this == HEARTS && c == DIAMONDS))
			{
				return true;
			}

			if (this == c)
			{
				return true;
			}

			return false;
		}
	}

	@Override
	public String toString()
	{
		String cardType;
		switch(this.value)
		{
			case 1:
				cardType = "A";
				break;
			case 11:
				cardType = "J";
				break;
			case 12:
				cardType = "Q";
				break;
			case 13:
				cardType = "K";
				break;
			default:
				cardType = this.value + "";
		}

		return cardType + "(" + this.color.toString() + ")";
	}

	@Override
	public boolean equals(Object obj)
	{
		if (!(obj instanceof Card))
		{
			return false;
		}

		if (((Card)obj).color != this.color || ((Card)obj).value != this.value)
		{
			return false;
		}

		return true;
	}

	@Override
	public int hashCode() {
		int hash = 1;
		hash = hash * 17 + this.color.hashCode();
		hash = hash * 31 + this.value;

		return hash;
    }

	public int value()
	{
		return this.value;
	}

	public boolean isTurnedFaceUp()
	{
		return this.faceUp;
	}

	public boolean turnFaceUp()
	{
		if (this.faceUp)
		{
			return false;
		}

		this.faceUp = true;
		return true;
	}

	public Card.Color color()
	{
		return this.color;
	}

	public boolean similarColorTo(Card c)
	{
		return this.color.similarColorTo(c.color);
	}

	public int compareValue(Card c)
	{
		return this.value - c.value;
	}
}
