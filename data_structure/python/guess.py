"""
To make sure that when input y/Y the game will restart
Input an int:50
wrong! Too big!
Input an int:25
wrong! Too small!
Input an int:40
wrong! Too big!
Input an int:32
wrong! Too big!
Input an int:29
wrong! Too small!
Input an int:31
you are right
Would you wanna continue?(Y/N):Y
Input an int:50
wrong! Too big!
Input an int:25
wrong! Too big!
Input an int:12
wrong! Too small!
Input an int:19
wrong! Too small!
Input an int:22
wrong! Too small!
Input an int:24
you are right
Would you wanna continue?(Y/N):noaaaaaa

Process finished with exit code 0

To make sure that it will be game over after 10 wrong tries
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Input an int:1
wrong! Too small!
Would you wanna continue?(Y/N):
"""
import random

def guess_game():
    while (1):
        try:
            answer_right = random.randint(1, 100)
            for i in range(10):
                answer_guess = int(input("Input an int:"))
                if answer_guess == answer_right:
                    print("you are right")
                    break
                elif answer_guess < answer_right:
                    print("wrong! Too small!")
                else:
                    print("wrong! Too big!")

            choice = input("Would you wanna continue?(Y/N):").lower()
            if choice == 'y':
                continue
            else:
                break

        except Exception:
            print("Some errors occur.")
            break

if __name__ == '__main__':
    guess_game()