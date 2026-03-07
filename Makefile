PRJNAME=jsprint

CXX=g++
CHKCXX=clang++

CXXFLAGS=
CXXFLAGSDEF=-std=c++20 -pedantic -Wall -Wextra -Werror
CXXFLAGSDEB=$(CXXFLAGSDEF) -g -pg -O0 -DDEBUG
CXXFLAGSREL=$(CXXFLAGSDEF) -O3 -DNDEBUG
LDLIBS=
LDFLAGS=

LDFLAGSDEF=-L/usr/lib -Wl,-rpath,'$$ORIGIN' -Wl,--as-needed -Llib -Lbin/

LDFLAGSDEB=$(LDFLAGSDEF) $(LDLIBS)
LDFLAGSREL=$(LDFLAGSDEF) $(LDLIBS)
LDFLAGSTST=

DEPDIR=bin
OUTDIR=trg
OUTDIRBIN=$(OUTDIR)/bin
OUTDIROBJ=$(OUTDIR)/obj
OUTDIREXE=$(PRJNAME)

SRC=src
SRCAPP=$(SRC)

SRCDIRSAPP  := $(shell find $(SRCAPP) -maxdepth 5 -type d)
SRCFILESAPP := $(foreach dir,$(SRCDIRSAPP),$(wildcard $(dir)/*.cpp))
OBJFILESAPP := $(addprefix $(OUTDIROBJ)/,$(subst $(SRCAPP)/, ,$(patsubst %.cpp,%.o,$(SRCFILESAPP))))

define set_real_src_file
    $(eval REAL_SRC_FILE=$(strip $(1)))
endef

define set_nothing
endef

define get_real_src_file
    $(if $(strip $(findstring $(strip $(1)),$(strip $(2)))),$(call set_real_src_file,$(2)),$(call set_nothing))
endef

define get_source_file
    $(eval REAL_SRC_SEARCH=$(notdir $(patsubst %.o,%.cpp,$(1))))
    $(eval REAL_SRC_FILE=)
    $(foreach word,$(2), $(call get_real_src_file, $(REAL_SRC_SEARCH),$(word)))
    $(if $(strip $(findstring $(PRJNAME),$(REAL_SRC_SEARCH))),$(call inc_build_version))
    @echo "Compile : $(REAL_SRC_FILE) -> $(1)"
endef

define create_object_dir
    @mkdir -p $(1)
endef

.PHONY: all

all: debug

debug:CXXFLAGS=$(CXXFLAGSDEB)
debug:LDFLAGS=$(LDFLAGSDEB)
debug:$(OBJFILESAPP)
	@mkdir -p $(OUTDIRBIN)
	@echo " Target :" $(OUTDIRBIN)/$(OUTDIREXE)
	@ $(CXX) -o $(OUTDIRBIN)/$(OUTDIREXE) $^ $(LDFLAGS)
	@mkdir -p $(DEPDIR)
	@ cp -f $(OUTDIRBIN)/$(OUTDIREXE) $(DEPDIR)

release:CXXFLAGS=$(CXXFLAGSREL)
release:LDFLAGS=$(LDFLAGSREL)
release:$(OBJFILESAPP)
	@mkdir -p $(OUTDIRBIN)
	@echo " Target :" $(OUTDIRBIN)/$(OUTDIREXE)
	@ $(CXX)  -o $(OUTDIRBIN)/$(OUTDIREXE) $^ $(LDFLAGS)
	@mkdir -p $(DEPDIR)
	@ cp -f $(OUTDIRBIN)/$(OUTDIREXE) $(DEPDIR)

$(OBJFILESAPP): $(SRCFILESAPP)
	$(call create_object_dir,$(@D))
	$(call get_source_file,$@,$^,$<)
	@ $(CXX) $(CXXFLAGS) -c $(REAL_SRC_FILE) -o $@

clean:
	@rm -rf $(OUTDIR)
