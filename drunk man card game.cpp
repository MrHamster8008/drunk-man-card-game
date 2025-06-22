#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

class FQueue
{
private:
    struct Node
    {
        int Data;
        Node* Next;
        Node(int Val) : Data(Val), Next(nullptr) {}
    };

    Node* FrontPtr;
    Node* LastPtr;

public:
    FQueue() : FrontPtr(nullptr), LastPtr(nullptr) {}

    ~FQueue()
    {
        while (FrontPtr != nullptr)
        {
            Node* Temp = FrontPtr;
            FrontPtr = FrontPtr->Next;
            delete Temp;
        }
        LastPtr = nullptr;
    }

    void Add(int Value)
    {
        Node* NewNode = new Node(Value);
        if (LastPtr == nullptr)
        {
            FrontPtr = LastPtr = NewNode;
        }
        else
        {
            LastPtr->Next = NewNode;
            LastPtr = NewNode;
        }
    }

    void Remove()
    {
        if (FrontPtr == nullptr) return;

        Node* Temp = FrontPtr;
        FrontPtr = FrontPtr->Next;

        if (FrontPtr == nullptr)
        {
            LastPtr = nullptr;
        }

        delete Temp;
    }

    int Front() const
    {
        return FrontPtr->Data;
    }

    bool IsEmpty() const
    {
        return FrontPtr == nullptr;
    }
};

string RandomDeck(string& CardsAvailable)
{
    string Deck;
    for (int i = 0; i < 5; i++)
    {
        int Position = rand() % CardsAvailable.size();
        Deck += CardsAvailable[Position];
        CardsAvailable.erase(Position, 1);
    }
    return Deck;
}

string YourDeck(string& CardsAvailable)
{
    string Deck;
    for (int i = 0; i < 5; )
    {
        string Input;
        cout << "Введите " << i << "-ю карту которую хотите добавить: " << endl;
        cout << "Доступные карты: " << CardsAvailable << endl;
        cin >> Input;

        if (Input.size() != 1 || !isdigit(Input[0]))
        {
            cout << "Ошибка: нужно ввести одну цифру от 0 до 9" << endl;
            continue;
        }

        char Card = Input[0];
        bool bCardFound = false;

        for (int j = 0; j < CardsAvailable.size(); j++)
        {
            if (CardsAvailable[j] == Card)
            {
                Deck += Card;
                CardsAvailable.erase(j, 1);
                bCardFound = true;
                i++; 
                break;
            }
        }

        if (!bCardFound)
        {
            cout << "Эта карта недоступна или уже использована, выберите другую" << endl;
        }
    }
    return Deck;
}

string DeckFromFile(string& CardsAvailable)
{
    string Deck;
    string Filename;
    ifstream File;

    while (true)
    {
        cout << "Введите имя файла: ";
        cin >> Filename;

        File.open(Filename);
        if (File.is_open())
        {
            break;
        }
        cout << "Ошибка: файл не найден. Попробуйте еще раз." << endl;
        File.clear();
    }

    char Card;
    int CardsRead = 0;
    bool bErrorInFile = false;

    while (File >> Card && CardsRead < 5 && !bErrorInFile)
    {
        if (!isdigit(Card))
        {
            cout << "Ошибка: файл содержит недопустимый символ '" << Card
                << "'. Допустимы только цифры 0-9." << endl;
            bErrorInFile = true;
            break;
        }

        bool bCardFound = false;
        for (int j = 0; j < CardsAvailable.size(); j++)
        {
            if (CardsAvailable[j] == Card)
            {
                Deck += Card;
                CardsAvailable.erase(j, 1);
                bCardFound = true;
                CardsRead++;
                break;
            }
        }

        if (!bCardFound)
        {
            cout << "Ошибка: карта '" << Card << "' уже использована или недоступна." << endl;
            bErrorInFile = true;
        }
    }

    File.close();

    if (CardsRead < 5 || bErrorInFile)
    {
        cout << "Файл содержит ошибки. Пожалуйста, выберите другой файл." << endl;
        for (char c : Deck)
        {
            CardsAvailable += c;
        }
        return DeckFromFile(CardsAvailable); 
    }

    return Deck;
}

int CorrectlyEnterChoice(int PlayerNum) {
    int Choice;
    while (true) {
        cout << "Как вы хотите заполнить колоду " << PlayerNum <<"-го игрока ? " << endl
            << "(1 - рандомно, 2 - с клавиатуры, 3 - из файла, остальное - закончить работу: ";

        if (cin >> Choice) {
            break;
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите число!" << endl;
        }
    }
    return Choice;
}



int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(0));
    string StartingDeck1, StartingDeck2, CardsAvailable = "0123456789";
    FQueue FirstPlayer, SecondPlayer;
    int Choice, Turn = 0, FirstPlayerCard, SecondPlayerCard;
    bool bFirstDeckIsEmpty = false, bSecondDeckIsEmpty = false;

    Choice = CorrectlyEnterChoice(1);
    switch (Choice)
    {
    case 1: StartingDeck1 = RandomDeck(CardsAvailable); break;
    case 2: StartingDeck1 = YourDeck(CardsAvailable); break;
    case 3: StartingDeck1 = DeckFromFile(CardsAvailable); break;
    default: break;
    }

    Choice = CorrectlyEnterChoice(2);
    switch (Choice)
    {
    case 1: StartingDeck2 = RandomDeck(CardsAvailable); break;
    case 2: StartingDeck2 = YourDeck(CardsAvailable); break;
    case 3: StartingDeck2 = DeckFromFile(CardsAvailable); break;
    default: break;
    }

    for (int i = 0; i < 5; i++)
    {
        FirstPlayer.Add(int(StartingDeck1[i]) - '0');
        SecondPlayer.Add(int(StartingDeck2[i]) - '0');
    }

    while (!bFirstDeckIsEmpty && !bSecondDeckIsEmpty && Turn < 1000000)
    {
        Turn++;
        FirstPlayerCard = FirstPlayer.Front();
        SecondPlayerCard = SecondPlayer.Front();
        FirstPlayer.Remove();
        SecondPlayer.Remove();

        if (FirstPlayerCard > SecondPlayerCard || (FirstPlayerCard == 0 && SecondPlayerCard == 9))
        {
            FirstPlayer.Add(FirstPlayerCard);
            FirstPlayer.Add(SecondPlayerCard);
        }
        else if (FirstPlayerCard < SecondPlayerCard || (FirstPlayerCard == 9 && SecondPlayerCard == 0))
        {
            SecondPlayer.Add(FirstPlayerCard);
            SecondPlayer.Add(SecondPlayerCard);
        }

        bFirstDeckIsEmpty = FirstPlayer.IsEmpty();
        bSecondDeckIsEmpty = SecondPlayer.IsEmpty();
    }

    if (bFirstDeckIsEmpty)
    {
        cout << "first" << endl << "Потребовалось ходов: " << Turn;
    }
    else if (bSecondDeckIsEmpty)
    {
        cout << "second" << endl << "Потребовалось ходов: " << Turn;
    }
    else
    {
        cout << "botva";
    }
}