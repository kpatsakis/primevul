  ProgramManager::ProgramInfo* GetProgramInfo(GLuint client_id) {
    ProgramManager::ProgramInfo* info =
        program_manager()->GetProgramInfo(client_id);
    return (info && !info->IsDeleted()) ? info : NULL;
  }
