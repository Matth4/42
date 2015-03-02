/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p4_getmove_bdurst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bdurst <bdurst@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/03 11:39:02 by bdurst            #+#    #+#             */
/*   Updated: 2015/02/04 19:05:42 by darresti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "puiss4.h"
#include <stdio.h>

#define ADV_COLOR(color)	color == ROUGE ? JAUNE : ROUGE
#define ABS(x)				((x) < 0 ? -(x) : (x))
#define IS_EMPTY(i, j)		((*grid)[i][j] == VIDE && (((i) == GRID_H - 1) ? 1 : (*grid)[(i)+1][j] != VIDE))

static void	print_tab(int play_tab[])
{
	int		i = 0;

	while (i < GRID_W)
	{
		printf("%d: %d, ", i, play_tab[i]);
		++i;
	}
	printf("\n");
}

static void	check_bud_line(t_grid *grid, int play_tab[], t_case color, int tip)
{
	int		i = GRID_H, j;

	while (i-- > 0)
	{
		j = 0;
		while (j < GRID_W - 1)
		{
			if ((*grid)[i][j] == color && (*grid)[i][j+1] == color)
			{
				if (!j)
				{
					if (IS_EMPTY(i, j+2) && IS_EMPTY(i, j+3))
						play_tab[j+3] += 20 + tip;
				}
				else if (j == GRID_W - 2)
				{
					if (IS_EMPTY(i, j-2) && IS_EMPTY(i, j-1))
						play_tab[j-2] += 20 + tip;
				}
				else if (j == 1)
				{
					if (IS_EMPTY(i, j-1) && IS_EMPTY(i, j+2) && IS_EMPTY(i, j+3))
						play_tab[j+2] += 1000 + tip;
					else if (IS_EMPTY(i, j+2))
					{
						if (IS_EMPTY(i, j-1))
							play_tab[j+2] += 20 + tip;
						else if (IS_EMPTY(i, j+3))
							play_tab[j+3] += 20 + tip;
					}
				}
				else if (j == (GRID_W - 3))
				{
					if (IS_EMPTY(i, j-2) && IS_EMPTY(i, j-1) && IS_EMPTY(i, j+2))
						play_tab[j-1] += 1000 + tip;
					else if (IS_EMPTY(i, j-1))
					{
						if (IS_EMPTY(i, j-2))
							play_tab[j-2] += 20 + tip;
						else if (IS_EMPTY(i, j+2))
							play_tab[j-1] += 20 + tip;
					}
				}
				else
				{
					if (IS_EMPTY(i, j-1) && IS_EMPTY(i, j+2) && (IS_EMPTY(i, j-2) || IS_EMPTY(i, j+3)))
					{
						if (IS_EMPTY(i, j-2))
							play_tab[j-1] += 1000 + tip;
						if (IS_EMPTY(i, j+3))
							play_tab[j+2] += 1000 + tip;
					}
					else
					{
						if (IS_EMPTY(i, j-2) && IS_EMPTY(i, j-1))
						{
							if (ABS((j-2) - (GRID_W/2)) <= ABS((j-1) - (GRID_W/2)))
								play_tab[j-2] += 20 + tip;
							else
								play_tab[j-1] += 20 + tip;
						}
						else if (IS_EMPTY(i, j-1) && IS_EMPTY(i, j+2))
						{
							if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
								play_tab[j-1] += 20 + tip;
							else
								play_tab[j+2] += 20 + tip;
						}
						else if (IS_EMPTY(i, j+2) && IS_EMPTY(i, j+3))
						{
							if (ABS((j+2) - (GRID_W/2)) <= ABS((j+3) - (GRID_W/2)))
								play_tab[j+2] += 20 + tip;
							else
								play_tab[j+3] += 20 + tip;
						}
					}
				}
			}
			++j;
		}
	}
}

static void	check_bud_diag_up(t_grid *grid, int play_tab[], t_case color, int tip)
{
	int		i = GRID_H, j;

	while (i-- > 1)
	{
		j = 0;
		while (j < GRID_W - 1)
		{
			if ((*grid)[i][j] == color && (*grid)[i-1][j+1] == color)
			{
				if (i == 1)
				{
					if (j > 1)
					{
						if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1))
						{
							if (ABS((j-2) - (GRID_W/2)) <= ABS((j-1) - (GRID_W/2)))
								play_tab[j-2] += 20 + tip;
							else
								play_tab[j-1] += 20 + tip;
						}
					}
				}
				else if (i == GRID_H - 1)
				{
					if (j < GRID_W - 3)
					{
						if (IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
						{
							if (ABS((j+2) - (GRID_W/2)) <= ABS((j+3) - (GRID_W/2)))
								play_tab[j+2] += 20 + tip;
							else
								play_tab[j+3] += 20 + tip;
						}
					}
				}
				else if (i == 2)
				{
					if (j == 1)
					{
						if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j+2] += 20 + tip;
					}
					else if (j == GRID_W - 2)
					{
						if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1))
							play_tab[j-2] += 20 + tip;
					}
					else if (j > 1)
					{
						if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j-1] += 1000 + tip;
						else if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
						{
							if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
								play_tab[j-1] += 20 + tip;
							else
								play_tab[j+2] += 20 + tip;
						}
					}
				}
				else if (i == GRID_H - 2)
				{
					if (!j)
					{
						if (IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
							play_tab[j+3] += 20 + tip;
					}
					else if (j == GRID_W - 3)
					{
						if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j-1] += 20 + tip;
					}
					else if (j < GRID_W - 3)
					{
						if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
							play_tab[j+2] += 1000 + tip;
						else if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
						{
							if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
								play_tab[j-1] += 20 + tip;
							else
								play_tab[j+2] += 20 + tip;
						}
					}
				}
				else
				{
					if (!j)
					{
						if (IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
							play_tab[j+3] += 20 + tip;
					}
					else if (j == GRID_W - 2)
					{
						if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1))
							play_tab[j-2] += 20 + tip;
					}
					else if (j == 1)
					{
						if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
							play_tab[j+2] += 1000 + tip;
						else if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j+2] += 20 + tip;
					}
					else if (j == GRID_W - 3)
					{
						if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j-1] += 1000 + tip;
						else if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							play_tab[j-1] += 20 + tip;
					}
					else
					{
						if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2) && (IS_EMPTY(i+2, j-2) || IS_EMPTY(i-3, j+3)))
						{
							if (IS_EMPTY(i+2, j-2))
								play_tab[j-1] += 1000 + tip;
							if (IS_EMPTY(i-3, j+3))
								play_tab[j+2] += 1000 + tip;
						}
						else
						{
							if (IS_EMPTY(i+2, j-2) && IS_EMPTY(i+1, j-1))
							{
								if (ABS((j-2) - (GRID_W/2)) <= ABS((j-1) - (GRID_W/2)))
									play_tab[j-2] += 20 + tip;
								else
									play_tab[j-1] += 20 + tip;
							}
							else if (IS_EMPTY(i+1, j-1) && IS_EMPTY(i-2, j+2))
							{
								if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
									play_tab[j-1] += 20 + tip;
								else
									play_tab[j+2] += 20 + tip;
							}
							else if (IS_EMPTY(i-2, j+2) && IS_EMPTY(i-3, j+3))
							{
								if (ABS((j+2) - (GRID_W/2)) <= ABS((j+3) - (GRID_W/2)))
									play_tab[j+2] += 20 + tip;
								else
									play_tab[j+3] += 20 + tip;
							}
						}
					}
				}
			}
			++j;
		}
	}
}

static void	check_bud_diag_down(t_grid *grid, int play_tab[], t_case color, int tip)
{
	int		i = GRID_H - 1, j;

	while (i-- > 0)
	{
		j = 0;
		while (j < GRID_W - 1)
		{
			if ((*grid)[i][j] == color && (*grid)[i+1][j+1] == color)
			{
				if (i == 0)
				{
					if (j < GRID_W - 3)
					{
						if (IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
						{
							if (ABS((j+2) - (GRID_W/2)) <= ABS((j+3) - (GRID_W/2)))
								play_tab[j+2] += 20 + tip;
							else
								play_tab[j+3] += 20 + tip;
						}
					}
				}
				else if (i == GRID_H - 2)
				{
					if (j > 1)
					{
						if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1))
						{
							if (ABS((j-2) - (GRID_W/2)) <= ABS((j-1) - (GRID_W/2)))
								play_tab[j-2] += 20 + tip;
							else
								play_tab[j-1] += 20 + tip;
						}
					}
				}
				else if (i == 1)
				{
					if (!j)
					{
						if (IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
							play_tab[j+3] += 20 + tip;
					}
					else if (j == GRID_W - 3)
					{
						if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j-1] += 20 + tip;
					}
					else if (j < GRID_W - 3)
					{
						if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
							play_tab[j+2] += 1000 + tip;
						else if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
						{
							if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
								play_tab[j-1] += 20 + tip;
							else
								play_tab[j+2] += 20 + tip;
						}
					}
				}
				else if (i == GRID_H - 3)
				{
					if (j == 1)
					{
						if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j+2] += 20 + tip;
					}
					else if (j == GRID_W - 2)
					{
						if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1))
							play_tab[j-2] += 20 + tip;
					}
					else if (j > 1)
					{
						if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j-1] += 1000 + tip;
						else if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
						{
							if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
								play_tab[j-1] += 20 + tip;
							else
								play_tab[j+2] += 20 + tip;
						}
					}
				}
				else
				{
					if (!j)
					{
						if (IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
							play_tab[j+3] += 20 + tip;
					}
					else if (j == GRID_W - 2)
					{
						if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1))
							play_tab[j-2] += 20 + tip;
					}
					else if (j == 1)
					{
						if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
							play_tab[j+2] += 1000 + tip;
						else if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j+2] += 20 + tip;
					}
					else if (j == GRID_W - 3)
					{
						if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j-1] += 1000 + tip;
						else if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							play_tab[j-1] += 20 + tip;
					}
					else
					{
						if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2) && (IS_EMPTY(i-2, j-2) || IS_EMPTY(i+3, j+3)))
						{
							if (IS_EMPTY(i-2, j-2))
								play_tab[j-1] += 1000 + tip;
							if (IS_EMPTY(i+3, j+3))
								play_tab[j+2] += 1000 + tip;
						}
						else
						{
							if (IS_EMPTY(i-2, j-2) && IS_EMPTY(i-1, j-1))
							{
								if (ABS((j-2) - (GRID_W/2)) <= ABS((j-1) - (GRID_W/2)))
									play_tab[j-2] += 20 + tip;
								else
									play_tab[j-1] += 20 + tip;
							}
							else if (IS_EMPTY(i-1, j-1) && IS_EMPTY(i+2, j+2))
							{
								if (ABS((j-1) - (GRID_W/2)) <= ABS((j+2) - (GRID_W/2)))
									play_tab[j-1] += 20 + tip;
								else
									play_tab[j+2] += 20 + tip;
							}
							else if (IS_EMPTY(i+2, j+2) && IS_EMPTY(i+3, j+3))
							{
								if (ABS((j+2) - (GRID_W/2)) <= ABS((j+3) - (GRID_W/2)))
									play_tab[j+2] += 20 + tip;
								else
									play_tab[j+3] += 20 + tip;
							}
						}
					}
				}
			}
			++j;
		}
	}
}

static void	check_bud(t_grid *grid, int play_tab[], t_case color, int tip)
{
	check_bud_line(grid, play_tab, color, tip);
	check_bud_diag_up(grid, play_tab, color, tip);
	check_bud_diag_down(grid, play_tab, color, tip);
}

static int	check_won(t_grid *grid, int play_tab[], t_case color)
{
	int		i = 0;

	while (i < GRID_W)
	{
		if (play_tab[i] != -1)
		{
			p4_play(grid, i, color);
			if (p4_won(grid, i))
				return (i);
			p4_unplay(grid, i, color);
		}
		++i;
	}
	return (-1);
}

static int	weight_tab(int play_tab[])
{
	int		i, j;
	int		weight_max = 0;
	int		count = 0;

	i = 0;
	while (i < GRID_W)
	{
		if (play_tab[i] > weight_max)
			weight_max = play_tab[i];
		++i;
	}
	i = 0;
	while (i < GRID_W)
		if (play_tab[i++] == weight_max)
			++count;
	if (count == 1)
	{
		i = 0;
		while (i < GRID_W)
		{
			if (play_tab[i] == weight_max)
				return (i);
			++i;
		}
	}
	else
	{
		i = GRID_W / 2;
		j = 0;
		while (j < GRID_W)
		{
			if (j % 2)
				i += j;
			else
				i -= j;
			if (play_tab[i] == weight_max)
				return (i);
			++j;
		}
	}
	return (-1); // Should never happen
}

int			p4_getmove_bdurst(t_grid *grid, t_case color, int turn_count)
{
	int		i, play_col;
	int		play_tab[GRID_W] = {0};

	(void)turn_count;

	/* Play middle column by default */
	if (GRID_W % 2)
		play_tab[GRID_W / 2] += 1;
	else
	{
		play_tab[GRID_W / 2] += 1;
		play_tab[GRID_W / 2 - 1] += 1;
	}

	/* Mark full columns as illegal */
	i = 0;
	while (i < GRID_W)
	{
		if (!p4_legal(grid, i))
			play_tab[i] = -1;
		++i;
	}

	/* Check for winning column */
	if ((play_col = check_won(grid, play_tab, color)) != -1)
	{
		printf("%d\n", play_col);
		return (play_col);
	}

	/* Check for losing column */
	if ((play_col = check_won(grid, play_tab, ADV_COLOR(color))) != -1)
	{
		printf("%d\n", play_col);
		return (play_col);
	}

	/* Check for 2 out of 4 aligned */
	check_bud(grid, play_tab, color, 1);
	check_bud(grid, play_tab, ADV_COLOR(color), 0);

	/* Look up the most promising column */
	play_col = weight_tab(play_tab);

	print_tab(play_tab);
	return (play_col);
}
