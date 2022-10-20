#! /bin/bash

ERROR=( "map/wrong_extension"                       \
        "map/wrong_extension.cub2"                  \
        "map/.cub"						            \
        "map/.cub.cub"						        \
        "map/no_file.cub"						    \
		"map/empty_file.cub"						\
		"map/error_no_EA_texture.cub"				\
		"map/error_wrong_EA_texture.cub"			\
		"map/error_no_WE_texture.cub"				\
		"map/error_wrong_WE_texture.cub"			\
		"map/error_no_SO_texture.cub"				\
		"map/error_wrong_SO_texture.cub"			\
		"map/error_no_NO_texture.cub"				\
		"map/error_wrong_NO_texture.cub"			\
		"map/error_no_F_color.cub"				    \
		"map/error_wrong_F_color.cub"				\
		"map/error_no_C_color.cub"			    	\
		"map/error_wrong_C_color.cub"				\
		"map/error_wrong_C_color2.cub"				\
		"map/error_wrong_C_color3.cub"				\
		"map/error_wrong_C_color4.cub"				\
		"map/error_duplicate_texture.cub"			\
		"map/error_duplicate_color.cub"				\
		"map/error_no_player_map.cub"				\
		"map/error_duplicate_player_map.cub"		\
		"map/error_wrong_element_map.cub"   		\
		"map/error_not_surrounded_by_wall_map.cub"  \
		"map/error_not_surrounded_by_wall_map2.cub" \
		)

my_cub3d()
{
	echo
	echo "CASE : $i"
	echo ---------------------------
	echo
    cat "$i"
	./cub3d "$i"
	echo
	echo ---------------------------
	echo
}

for i in "${ERROR[@]}"
do
	my_cub3d
done
