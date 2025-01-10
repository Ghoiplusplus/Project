package project.webclient.model.mainmoduleapi;

import lombok.Data;

@Data
public class Discipline {
    private Long id;
    private String name;
    private String description;
    private Long teacherId;
}
