// Fill out your copyright notice in the Description page of Project Settings.


#include "BullCowCartridge.h"       //while pressing ctrl + click takes us to the header file
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay()     // When the game starts
{
    Super::BeginPlay();
    isograms = getValidWords(Words);
    PrintLine(TEXT("Bulls mean letter in same place and correct\nCows mean letter is correct but wrong place."));
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking playerGuess
    {
        ProcessGuess(PlayerInput);
    }
}


void UBullCowCartridge::SetupGame()
{
    //PrintLine(TEXT("Cows are real!"));          // TEXT() macrosu unrealin düzgün bir şekilde encode edebilmesi için var.
    
    HiddenWord = isograms[FMath::RandRange(0, isograms.Num() - 1)];
    //PrintLine(TEXT("The Hidden Word is: %s."), *HiddenWord);                                //Debug Line

    Lives = Lives + HiddenWord.Len();

    PrintLine(TEXT("Guess %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Lives: %i "), Lives);
    PrintLine(TEXT("Write your guess and press Enter to continue"));


    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter To Play Again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(HiddenWord == Guess)
    {
        PrintLine(TEXT("You have Won!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i characters long."), HiddenWord.Len());
    }

    if(!IsIsogram(Guess))
    {
        //code
        PrintLine(TEXT("No repeating letters, this is not isogram, guess again!"));
    }

    PrintLine(TEXT("Lives Left: %i "), --Lives);

    if(Lives <= 0)
    {
        PrintLine(TEXT("You have no lives left"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() == Guess.Len())
    {
        FBullCowCount Score = GetBullCows(Guess);
        PrintLine(TEXT("You have %i Bulls %i Cows"), Score.Bulls, Score.Cows);
    }
}

bool UBullCowCartridge::IsIsogram(const FString& guess) const                //IsIsogram?
{
    
    for( int32 i=0; i < guess.Len() - 1; i++)
    {
        for (int j = guess.Len() - 1; j > i; j--)
        {
            if (guess[i] == guess[j])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::getValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString word : WordList)               //Stringin(word) içine atıyor array(WordList) bitene kadar sonra tek tek ValidWords'e ekliyor range based for loop
    {
        if (IsIsogram(word))                    //if isogram add to the new array
        {
            ValidWords.Emplace(word);            
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const //burada referans aldığımız için yukarıda BullCount/CowCount adrese gidip değiştiriliyor. Const da değil bunlar.
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;    //same place same letter
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;       //not same place but same letter
                break;            //Durması lazım çünkü bulduktan sonraki harfler gereksiz efor
            }
        }
    }
    return Count;
}
