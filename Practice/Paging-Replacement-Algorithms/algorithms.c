#include <stdio.h>

void display(int frames[], int f)
{
    int i;
    for (i = 0; i < f; i++)
    {
        if (frames[i] == -1)
            printf("- ");
        else
            printf("%d ", frames[i]);
    }
}

void fifo(int pages[], int n, int f)
{
    int frames[20], i, j, k = 0, hit, faults = 0;

    for (i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nFIFO Page Replacement\n");

    for (i = 0; i < n; i++)
    {
        hit = 0;

        for (j = 0; j < f; j++)
        {
            if (frames[j] == pages[i])
            {
                hit = 1;
                break;
            }
        }

        if (!hit)
        {
            frames[k] = pages[i];
            k = (k + 1) % f;
            faults++;
        }

        printf("Page %d: ", pages[i]);
        display(frames, f);
        printf("\n");
    }

    printf("Page Faults = %d\n", faults);
    printf("Page Hits = %d\n", n - faults);
}

void lru(int pages[], int n, int f)
{
    int frames[20], recent[20];
    int i, j, k, pos, min, hit, faults = 0;

    for (i = 0; i < f; i++)
    {
        frames[i] = -1;
        recent[i] = -1;
    }

    printf("\nLRU Page Replacement\n");

    for (i = 0; i < n; i++)
    {
        hit = 0;

        for (j = 0; j < f; j++)
        {
            if (frames[j] == pages[i])
            {
                hit = 1;
                recent[j] = i;
                break;
            }
        }

        if (!hit)
        {
            pos = -1;

            for (j = 0; j < f; j++)
            {
                if (frames[j] == -1)
                {
                    pos = j;
                    break;
                }
            }

            if (pos == -1)
            {
                min = recent[0];
                pos = 0;

                for (j = 1; j < f; j++)
                {
                    if (recent[j] < min)
                    {
                        min = recent[j];
                        pos = j;
                    }
                }
            }

            frames[pos] = pages[i];
            recent[pos] = i;
            faults++;
        }

        printf("Page %d: ", pages[i]);
        display(frames, f);
        printf("\n");
    }

    printf("Page Faults = %d\n", faults);
    printf("Page Hits = %d\n", n - faults);
}

void optimal(int pages[], int n, int f)
{
    int frames[20], i, j, k, pos, farthest, next, hit;
    int faults = 0;

    for (i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nOptimal Page Replacement\n");

    for (i = 0; i < n; i++)
    {
        hit = 0;

        for (j = 0; j < f; j++)
        {
            if (frames[j] == pages[i])
            {
                hit = 1;
                break;
            }
        }

        if (!hit)
        {
            pos = -1;

            for (j = 0; j < f; j++)
            {
                if (frames[j] == -1)
                {
                    pos = j;
                    break;
                }
            }

            if (pos == -1)
            {
                farthest = -1;

                for (j = 0; j < f; j++)
                {
                    next = n + 1;

                    for (k = i + 1; k < n; k++)
                    {
                        if (frames[j] == pages[k])
                        {
                            next = k;
                            break;
                        }
                    }

                    if (next > farthest)
                    {
                        farthest = next;
                        pos = j;
                    }
                }
            }

            frames[pos] = pages[i];
            faults++;
        }

        printf("Page %d: ", pages[i]);
        display(frames, f);
        printf("\n");
    }

    printf("Page Faults = %d\n", faults);
    printf("Page Hits = %d\n", n - faults);
}

int main()
{
    int pages[50], n, f, choice, i;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter page reference string:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &f);

    do
    {
        printf("\n===== PAGE REPLACEMENT =====\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                fifo(pages, n, f);
                break;

            case 2:
                lru(pages, n, f);
                break;

            case 3:
                optimal(pages, n, f);
                break;

            case 4:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 4);

    return 0;
}
