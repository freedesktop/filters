all_fragments+=palm

# -----------------------------------------------
# count = 1
T4_PALM = \
    writer_AportisDoc_PalmDB_File

# -----------------------------------------------
# count = 1
F4_PALM = \
    AportisDoc_Palm_DB

# -----------------------------------------------
# count = 0
L4_PALM =

# -----------------------------------------------
# count = 0
C4_PALM =

# -----------------------------------------------
TYPES_4fcfg_palm           = $(foreach,i,$(T4_PALM) types$/$i.xcu          )
FILTERS_4fcfg_palm         = $(foreach,i,$(F4_PALM) filters$/$i.xcu        )
FRAMELOADERS_4fcfg_palm    = $(foreach,i,$(L4_PALM) frameloaders$/$i.xcu   )
CONTENTHANDLERS_4fcfg_palm = $(foreach,i,$(C4_PALM) contenthandlers$/$i.xcu)

# -----------------------------------------------
# needed to get dependencies inside global makefile work!
ALL_4fcfg_palm = \
    $(TYPES_4fcfg_palm) \
    $(foreach,i,$(FILTERS_4fcfg_base) $(MISC)$/$i) \
    $(FRAMELOADERS_4fcfg_palm) \
    $(CONTENTHANDLERS_4fcfg_palm)
