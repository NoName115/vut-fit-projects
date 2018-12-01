package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;


public class CardStack
{
	private ArrayList<Card> deck;
	public boolean isWorkingPack;

	public CardStack(boolean workingPack)
	{
		this.deck = new ArrayList<Card>();
		this.isWorkingPack = workingPack;
	}

	public Card pop()
	{
		if (this.deck.size() <= 0)
		{
			return null;
		}

		Card tempCard = this.deck.get(this.size() - 1);
		this.deck.remove(this.size() - 1);

		return tempCard;
	}

	public CardStack pop(Card card)
	{
		int startIndex = -1;
		for (int i = 0; i < this.deck.size(); ++i)
		{
			if (this.deck.get(i).equals(card))
			{
				startIndex = i;
				break;
			}
		}

		if (startIndex == -1)
		{
			return null;
		}

		CardStack newStack = new CardStack(false);
		int stackSize = this.deck.size();

		for (int i = startIndex; i < stackSize; ++i)
		{
			newStack.put(this.deck.get(startIndex));
			this.deck.remove(startIndex);
		}

		return newStack;
	}

	public boolean put(Card card)
	{
		if (this.isWorkingPack)
		{
			if (this.isEmpty())
			{
				// Nieje to KING
				if (card.value() != 13)
				{
					return false;
				}
			}
			else
			{
				// Hodnota o 1 nizsia a rozna farba
				int lastCardValue = this.deck.get(this.size() - 1).value();
				if (lastCardValue - card.value() != 1 ||
					this.deck.get(this.size() - 1).similarColorTo(card))
				{
					return false;
				}
			}
		}

		this.deck.add(card);
		return true;
	}

	public boolean put(CardStack stack)
	{
		if (this.isEmpty())
		{
			return false;
		}

		if (stack.isEmpty())
		{
			return true;
		}

		ArrayList<Card> inputList = stack.getList();
		while (!inputList.isEmpty())
		{
			int lastIndex = inputList.size() - 1;
			this.deck.add(inputList.get(lastIndex));
			inputList.remove(lastIndex);
		}

		return true;
	}

	public int size()
	{
		return this.deck.size();
	}

	public boolean isEmpty()
	{
		return this.deck.isEmpty();
	}

	public ArrayList<Card> getList()
	{
		return this.deck;
	}
}
