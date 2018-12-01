package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;


public class CardDeck
{

    private Card.Color color;
    private ArrayList<Card> cards;

    public CardDeck(Card.Color color)
    {
        this.cards = new ArrayList<Card>();
        this.color = color;
    }

    public Card get()
    {
        if (this.cards.isEmpty())
        {
            return null;
        }

        return this.cards.get(this.cards.size() - 1);
    }

    public Card get(int index)
    {
        if (this.cards.isEmpty())
        {
            return null;
        }

        return this.cards.get(index);
    }

    public boolean isEmpty()
    {
        return this.cards.isEmpty();
    }

    public Card pop()
    {
        if (this.cards.size() <= 0)
        {
            return null;
        }

        int lastIndex = this.cards.size() - 1;
        Card tempCard = this.cards.get(lastIndex);
        this.cards.remove(lastIndex);

        return tempCard;
    }

    public boolean put(Card card)
    {
        if (this.color != null)
        {
            if (!this.color.similarColorTo(card.color()))
            {
                return false;
            }

            if (this.isEmpty())
            {
                if (card.value() != 1)
                {
                    return false;
                }

                this.cards.add(card);
                return true;
            }

            if (card.value() - this.cards.get(this.size() - 1).value() != 1)
            {
                return false;
            }
        }

        this.cards.add(card);
        return true;
    }

    public int size()
    {
        return this.cards.size();
    }
}
