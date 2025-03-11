/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaladro <dbaladro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:46:34 by dbaladro          #+#    #+#             */
/*   Updated: 2025/03/11 18:51:36 by dbaladro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/atm328p.h"

/**
 * @brief Check if cmd is a known command
 *
 * @return 1 is yes, else 0
 */
uint8_t	is_valid_cmd(const unsigned char *cmd) {
	if (!ft_strcmp((const unsigned char *)"READ", cmd))
		return (1);
	if (!ft_strcmp((const unsigned char *)"WRITE", cmd))
		return (1);
	if (!ft_strcmp((const unsigned char *)"PRINT", cmd))
		return (1);
	if (!ft_strcmp((const unsigned char *)"FORGET", cmd))
		return (1);
	return (0);
}

/**
 * @brief Extract command part from readline
 *
 * @param cmd -- Readline
 * @param buff -- buffer to store command part
 */
void	get_cmd_part(const unsigned char *cmd, unsigned char *buff) {
	uint8_t	i = 0;

	while (i < DATA_MAX_SIZE - 1 && cmd[i] && cmd[i] != ' ') {
		buff[i] = cmd[i];
		i++;
	}
}

/**
 * @brief Extract param part from readline
 *
 * @param cmd -- Readline
 * @param buff -- buffer to store command part
 * @return uint8_t -- 1 on error, 0 on success
 */
uint8_t	get_param(const unsigned char *cmd, unsigned char *buff, uint8_t *pos) {
	uint8_t	buff_i = 0;

	if (*pos == READ_BUFFER_SIZE)
		return (1);
	if (cmd[*pos] != '\"')
		return (1);
	(*pos)++;
	while (*pos < READ_BUFFER_SIZE && buff_i < DATA_MAX_SIZE - 1 && cmd[*pos] && cmd[*pos] != '\"') {
		buff[buff_i++] = cmd[*pos];
		(*pos)++;
	}
	if (*pos == READ_BUFFER_SIZE || cmd[*pos] != '\"')
		return (1);
	(*pos)++;
	return (0);
}

/**
 * @brief Skip space in cmd starting from pos
 *
 * @param cmd -- Readline result
 * @param pos -- position in cmd
 */
void	skip_space(const unsigned char *cmd, uint8_t *pos) {
	while (*pos < READ_BUFFER_SIZE && cmd[*pos] == ' ')
		(*pos)++;
}

/**
 * @brief parse readed command
 *
 * @param cmd -- The command to parse
 * @param cmd_buff -- Buffer for command part
 * @param prm_1_buff -- Buffer for param 1
 * @param prm_2_buff -- Buffer for param 2
 * @return 0 on success, else 1
 */
uint8_t	parse_cmd(const unsigned char *cmd, unsigned char *cmd_buff,
					unsigned char *prm_1_buff, unsigned char *prm_2_buff) {
	uint8_t	cmd_i = 0;

	get_cmd_part(cmd, cmd_buff); /* Extract command part */
	if (!is_valid_cmd(cmd_buff)) /* Check for validity */
		return (1);
	cmd_i = ft_strlen(cmd_buff); /* Set cmd_i at end of word */
	skip_space(cmd, &cmd_i); /* Skip space */

	/* Check if command is PRINT and have parameter */
	if (!ft_strcmp(cmd_buff, (const unsigned char *)"PRINT"))
		return (cmd[cmd_i] != 0);
	
	if (get_param(cmd, prm_1_buff, &cmd_i)) /* Extract first param part */
		return (1);
	if (ft_strlen(prm_1_buff) == 0)
		return (1);
	skip_space(cmd, &cmd_i);

    /* Check if FORGET or READ Have too much parameter */
	if (ft_strcmp((const unsigned char *)"WRITE", cmd_buff) != 0) /* Valid READ / FORGET */
		return (cmd[cmd_i] != 0);

	if (get_param(cmd, prm_2_buff, &cmd_i))
		return (1);
	if (ft_strlen(prm_2_buff) == 0)
		return (1);

	skip_space(cmd, &cmd_i);
	return (cmd_i == DATA_MAX_SIZE || cmd[cmd_i] != 0);
}
