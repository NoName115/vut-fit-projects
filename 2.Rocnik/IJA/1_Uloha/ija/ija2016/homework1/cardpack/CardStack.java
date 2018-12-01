package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;


public class CardStack
{
	private int size;
	private ArrayList<Card> deck;

	public CardStack(int size)
	{
		this.size = size;
		this.deck = new ArrayList<Card>();
	}

	public void put(Card card)
	{
		if (this.deck.size() == this.size)
		{
			return;
		}

		this.deck.add(card);
	}

	public void put(CardStack stack)
	{
		ArrayList<Card> inputList = stack.getList();
		while (!inputList.isEmpty())
		{
			int lastIndex = inputList.size() - 1;
			this.deck.add(inputList.get(lastIndex));
			inputList.remove(lastIndex);
		}
	}

	public boolean isEmpty()
	{
		return this.deck.isEmpty();
	}

	public int size()
	{
		return this.deck.size();
	}

	public CardStack takeFrom(Card card)
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

		CardStack newStack = new CardStack(this.deck.size() - startIndex);
		int stackSize = this.deck.size();

		for (int i = startIndex; i < stackSize; ++i)
		{
			newStack.put(this.deck.get(startIndex));
			this.deck.remove(startIndex);
		}

		return newStack;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (!(obj instanceof CardStack))
		{
			return false;
		}

		if (this.size() != ((CardStack)obj).size())
		{
			return false;
		}

		for (int i = 0; i < this.size(); ++i)
		{
			if (!this.deck.get(i).equals(((CardStack)obj).getList().get(i)))
			{
				return false;
			}
		}

		return true;
	}

	private ArrayList<Card> getList()
	{
		return this.deck;
	}
}