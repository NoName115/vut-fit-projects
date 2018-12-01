package ija.ija2016.homework1.cardpack;

import java.lang.IllegalArgumentException;


public class Card
{
	private Card.Color color;
	private int value;

	public Card(Card.Color c, int value)
	{
		this.color = c;
		this.value = value;
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

	public Card.Color color()
	{
		return this.color;
	}

	public int value()
	{
		return this.value;
	}
}