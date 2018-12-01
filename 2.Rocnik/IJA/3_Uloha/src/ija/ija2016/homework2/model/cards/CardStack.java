package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;


public class CardStack
{
    private ArrayList<Card> cards;
    private boolean wPack;

    public CardStack(boolean wPack)
    {
        this.cards = new ArrayList<Card>();
        this.wPack = wPack;
    }

    public int size()
    {
        return this.cards.size();
    }

    public boolean isEmpty()
    {
        return this.cards.isEmpty();
    }

    public ArrayList<Card> getList()
    {
        return this.cards;
    }

    public Card pop()
    {
        if (this.cards.size() < 1)
        {
            return null;
        }

        Card tempCard = this.cards.get(this.size() - 1);
        this.cards.remove(this.size() - 1);

        return tempCard;
    }

    public CardStack pop(Card card)
    {
        int startIndex = -1;
        for (int i = 0; i < this.cards.size(); i++)
        {
            if (this.cards.get(i).equals(card))
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
        int stackSize = this.cards.size();

        for (int i = startIndex; i < stackSize; ++i)
        {
            newStack.put(this.cards.get(startIndex));
            this.cards.remove(startIndex);
        }

        return newStack;
    }

    public boolean put(Card card)
    {
        if (this.wPack)
        {
            if (this.isEmpty())
            {
                if (card.value() != 13)
                {
                    return false;
                }
            }
            else
            {
                int lastCardValue = this.cards.get(this.size() - 1).value();
                if (lastCardValue - card.value() != 1 ||
                        this.cards.get(this.size() - 1).similarColorTo(card))
                {
                    return false;
                }
            }
        }

        this.cards.add(card);
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
            this.cards.add(inputList.get(lastIndex));
            inputList.remove(lastIndex);
        }

        return true;
    }
}