package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

public class FactoryKlondike extends AbstractFactorySolitaire
{

    @Override
    public Card createCard(Card.Color color, int value)
    {
        if (value > 13 || value < 1) return null;
        return new Card(color, value);
    }

    @Override
    public CardDeck createCardDeck()
    {
        CardDeck newDeck = new CardDeck(null);
        for (Card.Color c : Card.Color.values())
        {
            for (int i = 1; i <= 13; i++)
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